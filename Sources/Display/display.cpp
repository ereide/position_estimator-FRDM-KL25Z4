/*
 * display.c
 *
 *  Created on: Feb 8, 2018
 *      Author: Eivind Roson Eide
 */

#include "display.h"
#include "Adafruit_SSD1331.h"

#define GREEN 0x07E0


Adafruit_SSD1331 oled_display;


void init_display(){
	oled_display.begin();
}


void draw_something(){
	oled_display.fillScreen(GREEN);
}

