// This file is part of CasaSoft Arduino SlideShow
//
// copyright (c) 2019 Roberto Ceccarelli - Casasoft
// 
// CasaSoft Arduino SlideShow is free software: 
// you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// CasaSoft Arduino SlideShow is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with CasaSoft Arduino SlideShow.  
// If not, see <http://www.gnu.org/licenses/>.

// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// adapted from (c) ladyada / adafruit

#include "Arduino.h"

#include "TouchScreen_kbv.h"

#define NUMSAMPLES 3  //.kbv
#if defined(__STM32F1__) || defined(ESP32)   //Maple core
#define ADC_ADJUST >>2
#else
#define ADC_ADJUST
#endif

TSPoint_kbv::TSPoint_kbv(void) {
	x = y = 0;
}

TSPoint_kbv::TSPoint_kbv(int16_t x0, int16_t y0, int16_t z0) {
	x = x0;
	y = y0;
	z = z0;
}

bool TSPoint_kbv::operator==(TSPoint_kbv p1) {
	return  ((p1.x == x) && (p1.y == y) && (p1.z == z));
}

bool TSPoint_kbv::operator!=(TSPoint_kbv p1) {
	return  ((p1.x != x) || (p1.y != y) || (p1.z != z));
}

static void insert_sort(int array[], uint8_t size) {
	uint8_t j;
	int save;

	for (int i = 1; i < size; i++) {
		save = array[i];
		for (j = i; j >= 1 && save < array[j - 1]; j--)
			array[j] = array[j - 1];
		array[j] = save;
	}
}

TSPoint_kbv TouchScreen_kbv::getPoint(void) {
	int x, y, z;
	int samples[NUMSAMPLES];
	uint8_t i, valid;

	valid = 1;

	pinMode(_yp, INPUT);
	pinMode(_ym, INPUT);

	digitalWrite(_yp, LOW);
	digitalWrite(_ym, LOW);

	pinMode(_xp, OUTPUT);
	pinMode(_xm, OUTPUT);
	digitalWrite(_xp, HIGH);
	digitalWrite(_xm, LOW);

	for (i = 0; i < NUMSAMPLES; i++) {
		samples[i] = analogRead(_yp) ADC_ADJUST;
	}
	insert_sort(samples, NUMSAMPLES);
	x = (1023 - samples[NUMSAMPLES / 2]); //choose median

	pinMode(_xp, INPUT);
	pinMode(_xm, INPUT);
	digitalWrite(_xp, LOW);
	digitalWrite(_xm, LOW);     //.kbv for Due

	pinMode(_yp, OUTPUT);
	digitalWrite(_yp, HIGH);
	pinMode(_ym, OUTPUT);
	digitalWrite(_ym, LOW);     //.kbv for Due

	for (i = 0; i < NUMSAMPLES; i++) {
		samples[i] = analogRead(_xm) ADC_ADJUST;
	}

	insert_sort(samples, NUMSAMPLES);

	y = (1023 - samples[NUMSAMPLES / 2]);

	// Set X+ to ground
	pinMode(_xp, OUTPUT);
	digitalWrite(_xp, LOW);

	// Set Y- to VCC
	pinMode(_ym, OUTPUT);     //.kbv
	digitalWrite(_ym, HIGH);

	// Hi-Z X- and Y+
	digitalWrite(_xm, LOW);   //.kbv
	pinMode(_xm, INPUT);      //.kbv
	digitalWrite(_yp, LOW);
	pinMode(_yp, INPUT);

	int z1 = analogRead(_xm) ADC_ADJUST;
	int z2 = analogRead(_yp) ADC_ADJUST;

	z = (1023 - (z2 - z1));

	pinMode(_yp, OUTPUT);      //restore shared pins
	pinMode(_xm, OUTPUT);
	digitalWrite(_yp, HIGH);  //because TFT control pins
	digitalWrite(_xm, HIGH);

	return TSPoint_kbv(x, y, z);  //XM, YP still in ANALOG mode
}

void TouchScreen_kbv::readResistiveTouch(void)
{
	tp = getPoint();
	pinMode(_yp, OUTPUT);      //restore shared pins
	pinMode(_xm, OUTPUT);
	digitalWrite(_yp, HIGH);  //because TFT control pins
	digitalWrite(_xm, HIGH);

}

bool TouchScreen_kbv::ISPRESSED(void)
{
	// .kbv this was too sensitive !!
	// now touch has to be stable for 50ms
	int count = 0;
	bool state, oldstate;
	while (count < 10) {
		readResistiveTouch();
		state = tp.z > 100;     //ADJUST THIS VALUE TO SUIT YOUR SCREEN e.g. 20 ... 250
		if (state == oldstate) count++;
		else count = 0;
		oldstate = state;
		delay(5);
	}
	return oldstate;

}

boolean TouchScreen_kbv::diagnose_pins(void)
{
	int i, j, value, Apins[2], Dpins[2], Values[2], found = 0;
	for (i = A0; i < A5; i++) pinMode(i, INPUT_PULLUP);
	for (i = 2; i < 10; i++) pinMode(i, INPUT_PULLUP);
	for (i = A0; i < A4; i++) {
		pinMode(i, INPUT_PULLUP);
		for (j = 5; j < 10; j++) {
			pinMode(j, OUTPUT);
			digitalWrite(j, LOW);
			value = analogRead(i);               // ignore first reading
			value = analogRead(i);
			if (value < 100 && value > 0) {
				if (found < 2) {
					Apins[found] = i;
					Dpins[found] = j;
					Values[found] = value;
				}
				found++;
			}
			pinMode(j, INPUT_PULLUP);
		}
		pinMode(i, INPUT_PULLUP);
	}
	if (found == 2) {
		int idx = Values[0] < Values[1];
		_xm = Apins[!idx]; _xp = Dpins[!idx]; _yp = Apins[idx]; _ym = Dpins[idx];
		return true;                              //success
	}
	return false;
}

TouchScreen_kbv::TouchScreen_kbv()
{
	// use default values
	_yp = A3;
	_xm = A2;
	_ym = 9;
	_xp = 8;
	_rxplate = 0;
	pressureThreshhold = 10;
}

TouchScreen_kbv::TouchScreen_kbv(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym) {
	_yp = yp;
	_xm = xm;
	_ym = ym;
	_xp = xp;
	_rxplate = 0;
	pressureThreshhold = 10;
}


TouchScreen_kbv::TouchScreen_kbv(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym,
	uint16_t rxplate) {
	_yp = yp;
	_xm = xm;
	_ym = ym;
	_xp = xp;
	_rxplate = rxplate;

	pressureThreshhold = 10;
}

int TouchScreen_kbv::readTouchX(void) {
	pinMode(_yp, INPUT);
	pinMode(_ym, INPUT);
	digitalWrite(_yp, LOW);
	digitalWrite(_ym, LOW);

	pinMode(_xp, OUTPUT);
	digitalWrite(_xp, HIGH);
	pinMode(_xm, OUTPUT);
	digitalWrite(_xm, LOW);

	return (1023 - (analogRead(_yp)) ADC_ADJUST);
}


int TouchScreen_kbv::readTouchY(void) {
	pinMode(_xp, INPUT);
	pinMode(_xm, INPUT);
	digitalWrite(_xp, LOW);
	digitalWrite(_xm, LOW);

	pinMode(_yp, OUTPUT);
	digitalWrite(_yp, HIGH);
	pinMode(_ym, OUTPUT);
	digitalWrite(_ym, LOW);

	return (1023 - (analogRead(_xm)) ADC_ADJUST);
}


uint16_t TouchScreen_kbv::pressure(void) {
	// Set X+ to ground
	pinMode(_xp, OUTPUT);
	digitalWrite(_xp, LOW);

	// Set Y- to VCC
	pinMode(_ym, OUTPUT);
	digitalWrite(_ym, HIGH);

	// Hi-Z X- and Y+
	digitalWrite(_xm, LOW);
	pinMode(_xm, INPUT);
	digitalWrite(_yp, LOW);
	pinMode(_yp, INPUT);

	int z1 = analogRead(_xm) ADC_ADJUST;
	int z2 = analogRead(_yp) ADC_ADJUST;

	return (1023 - (z2 - z1));
}
