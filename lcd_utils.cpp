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

#include <Arduino.h>
#include "lcd_utils.h"

// touchscreen utils
const int TS_LEFT = 110, TS_RT = 858, TS_TOP = 85, TS_BOT = 890;

HWClass HW;


void HWClass::init()
{
#ifdef DEBUG
	Serial.begin(9600);
#endif // DEBUG

	ts.diagnose_pins();
#ifdef DEBUG
	Serial.println(F("TS pins detected"));
#endif // DEBUG

	displayID = tft.readID();
#ifdef DEBUG
	Serial.print(F("Display type ID: "));
	Serial.println(displayID);
#endif // DEBUG

	if (displayID == 0x0D3D3) displayID = 0x9481;
	tft.begin(displayID);
#ifdef DEBUG
	Serial.println(F("Display started"));
#endif // DEBUG

	tft.setRotation(1);
	tft.fillScreen(TFT_BLUE);
	tft.setTextColor(TFT_YELLOW);
	tft.setTextSize(3);
}


void HWClass::getPointXY(void)
{
	// LANDSCAPE CALIBRATION    320 x 240
	point.y = map(ts.tp.x, TS_RT, TS_LEFT, 0, 240);
	point.x = map(ts.tp.y, TS_TOP, TS_BOT, 0, 320);
}
