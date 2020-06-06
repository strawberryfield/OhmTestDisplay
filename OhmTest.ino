/*
	Name:       OhmTest.ino
	Arduino Multimeter
*/

// Funzioni per pilotare il display
#include "lcd_utils.h"
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBoldOblique24pt7b.h>

// Parametri elettrici e taratura convertitore A/D
#define MAXVAL		1024.0
#define VCC			5.0
#define LOWBIAS		58
#define HIGHBIAS	950
#define REFRESISTOR	1000

int oldX = -1;

// Funzione per mostrare i risultati
void show(int x)
{
	char str[20];
	char vstr[10];

	// Gestisco la soglia bassa di lettura
	if (x <= LOWBIAS) {
		x = 0;
	}

	// Gestisco il limite di lettura
	if (x >= HIGHBIAS) {
		x = MAXVAL;
	}

	// se i valori non sono cambiati evito di riscriverli
	if (x != oldX) {
		oldX = x;
	}
	else return;

	// Calcolo la tensione ai capi della resistenza di riferimento
	float v = x / MAXVAL * VCC;

	// cancello i vecchi valori
	HW.tft.fillRect(70, 60, 150, 120, TFT_BLUE);

	// Mostro il valore della tensione ai capi delle resistenza in misura
	dtostrf(VCC - v, 6, 2, vstr);
	sprintf(str, "V= %s V", vstr);
	HW.tft.setCursor(30, 90);
	HW.tft.print(str);

	// Mostro la corrente che circola nel circuito
	dtostrf(v, 6, 2, vstr);
	sprintf(str, "I= %s mA", vstr);
	HW.tft.setCursor(30, 130);
	HW.tft.print(str);

	// Se non c'è la resistenza evito una divisione per 0
	if (x > 0) {
		dtostrf((VCC - v) / v * REFRESISTOR, 6, 0, vstr);
	}
	else {
		strncpy(vstr, "------", 6);
	}
	// Mostro il valore della resistenza
	sprintf(str, "R= %s Ohm", vstr);
	HW.tft.setCursor(30, 170);
	HW.tft.print(str);
}

// Funzione standard di Arduino per inizializzare il dispositivo
void setup()
{
	HW.init();
	HW.tft.fillScreen(TFT_BLUE);
	HW.tft.setCursor(20, 25);
	HW.tft.setFont(&FreeMonoBoldOblique24pt7b);
	HW.tft.setTextSize(1);
	HW.tft.setTextColor(TFT_YELLOW, TFT_WHITE);
	HW.tft.print("Multimetro");
	HW.tft.setFont(&FreeMonoBold18pt7b);


}

// Funzione standard di Arduino ripetuta all'infinito
void loop()
{
	int x = analogRead(A7);
	show(x);
	delay(500);
}
