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

#ifndef _LCD_UTILS_H_
#define _LCD_UTILS_H_

//#define DEBUG

#include <Arduino.h>

#include <Adafruit_GFX.h>   // Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include "TouchScreen_kbv.h" 

class HWClass
{
private:

public:
	MCUFRIEND_kbv tft;
	TouchScreen_kbv ts;   //re-initialised after diagnose
	TSPoint_kbv tp;
	TSPoint_kbv point;
	uint16_t displayID;

	void init();
	void getPointXY(void);
};

extern HWClass HW;

#endif
