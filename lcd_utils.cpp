#include <Arduino.h>
#include "lcd_utils.h"

void lcd_setup() {
    uint16_t ID;
    ID = tft.readID();
    if (ID == 0x0D3D3) ID = 0x9481;
    tft.begin(ID);
    tft.setRotation(1);
    tft.fillScreen(TFT_BLUE);
    tft.setTextColor(TFT_YELLOW);
}



// Touchscreen utils

void readResistiveTouch(void)
{
    tp = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);  //because TFT control pins
    digitalWrite(XM, HIGH);
}

bool ISPRESSED(void)
{
    // .kbv this was too sensitive !!
    // now touch has to be stable for 50ms
    int count = 0;
    bool state, oldstate;
    while (count < 10) {
        readResistiveTouch();
        state = tp.z > 200;     //ADJUST THIS VALUE TO SUIT YOUR SCREEN e.g. 20 ... 250
        if (state == oldstate) count++;
        else count = 0;
        oldstate = state;
        delay(5);
    }
    return oldstate;
}

boolean diagnose_pins()
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
        XM = Apins[!idx]; XP = Dpins[!idx]; YP = Apins[idx]; YM = Dpins[idx];
//        ts = TouchScreen(XP, YP, XM, YM, 300);    //re-initialise with pins
        ts = TouchScreen_kbv(XP, YP, XM, YM, 300);    //re-initialise with pins
        return true;                              //success
    }
    return false;
}
