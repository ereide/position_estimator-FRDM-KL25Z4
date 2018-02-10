/*
 * display.c
 *
 *  Created on: Feb 8, 2018
 *      Author: Eivind Roson Eide
 */

#include "display.h"

#include "oled_spi.h"
#include "Adafruit_SSD1331.h"


Adafruit_SSD1331 oled_display;


void init_display(void){
	//Initialize display
	oled_spi_init();

	//Startup screen
	oled_display.begin();

	oled_display.fillScreen(WHITE);

}

