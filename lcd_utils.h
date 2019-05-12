#ifndef _LCD_UTILS_H_
#define _LCD_UTILS_H_

#include <Arduino.h>

#include <Adafruit_GFX.h>   // Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include "TouchScreen_kbv.h" 


extern MCUFRIEND_kbv tft;
extern TouchScreen_kbv ts;   
extern TSPoint_kbv tp; 
extern int XP;
extern int YP;
extern int XM;
extern int YM;

void lcd_setup();


void readResistiveTouch(void);
bool ISPRESSED(void);
boolean diagnose_pins(void);
#endif
