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
// (c) ladyada / adafruit
// Original code under MIT License

#ifndef _TOUCHSCREEN_KBV_H_
#define _TOUCHSCREEN_KBV_H_
#include <stdint.h>

class TSPoint_kbv {
public:
	TSPoint_kbv(void);
	TSPoint_kbv(int16_t x, int16_t y, int16_t z);

	bool operator==(TSPoint_kbv);
	bool operator!=(TSPoint_kbv);

	int16_t x, y, z;
};

class TouchScreen_kbv {
public:
	TouchScreen_kbv();
	TouchScreen_kbv(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym);
	TouchScreen_kbv(uint8_t xp, uint8_t yp, uint8_t xm, uint8_t ym, uint16_t rx);

	TSPoint_kbv tp;
	uint16_t pressure(void);
	int readTouchY();
	int readTouchX();
	TSPoint_kbv getPoint();
	int16_t pressureThreshhold;
	void readResistiveTouch(void);
	bool ISPRESSED(void);
	boolean diagnose_pins(void);


private:
	uint8_t _yp, _ym, _xm, _xp;
	uint16_t _rxplate;
};

#endif

