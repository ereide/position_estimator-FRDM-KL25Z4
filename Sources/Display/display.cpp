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
}


void draw_something(void){
	oled_display.fillScreen(GREEN);
}

void draw_something_else(void){
	oled_display.fillScreen(BLUE);
}

