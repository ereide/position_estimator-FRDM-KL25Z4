/*
 * display.c
 *
 *	API for displaying data on the display
 *
 *  Created on: Feb 8, 2018
 *      Author: Eivind Roson Eide
 */


///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////


#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "display.h"
#include "utils.h"

#include "oled_spi.h"
#include "Adafruit_SSD1331.h"




////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////

#define DISPLAY_OUTPUT_STR_MAX_LENGTH 	20
#define DISPLAY_TEXT_SIZE 				1u
#define DISPLAY_TEXT_COLOR  			WHITE
#define DISPLAY_BACKGROUND_COLOR 		BLACK
#define DISPLAY_TEXT_WRAP				0


#define OK_CHAR							"OK"
#define ERR_CHAR						"NO"
#define EMPTY_VAL						" "


////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define BOOL_TO_C(X)  (X ? (OK_CHAR) : (ERR_CHAR))



///////////////////////////////////////////////////////////////////////////////
//  Code
///////////////////////////////////////////////////////////////////////////////



Adafruit_SSD1331 oled_display;

char output_str[DISPLAY_OUTPUT_STR_MAX_LENGTH];


int16_t data_cursor_x, data_cursor_y;



static void reset_cursor(void){
	int16_t x0 = 0; //oled_display.width()/16;
	int16_t y0 = oled_display.height();
	oled_display.setCursor(x0, y0);
}

static void reset_cursor_data(void){
	oled_display.setCursor(data_cursor_x, data_cursor_y);
}

static void reset_cursor_status(void){
	reset_cursor();
}

static void configure_print(void){

	//Sets font to default
	oled_display.setFont();

	reset_cursor();

	oled_display.setTextColor(DISPLAY_TEXT_COLOR, DISPLAY_BACKGROUND_COLOR);
	oled_display.setTextSize(DISPLAY_TEXT_SIZE);
	oled_display.setTextWrap(DISPLAY_TEXT_WRAP);

	//Finds the location of the third line
	oled_display.write('\n');

	oled_display.write('B');

	//oled_display.write('\n');
	//oled_display.write('\n');
	//oled_display.write('\n');

	data_cursor_x = oled_display.getCursorX();
	data_cursor_y = oled_display.getCursorY();

	reset_cursor();
}

void display_fill_screen(void){
	oled_display.fillScreen(WHITE);
	reset_cursor();
}


void display_empty_screen(void){
	oled_display.fillScreen(BLACK);
	reset_cursor();
}

void init_display(sys_status_t* status){
	//Initialize display
	oled_spi_init();

	//Startup screen
	oled_display.begin();

	oled_display.fillScreen(BLACK);

	configure_print();
}


void display_write_text(char* str){

	size_t n = strlen(str);

	//Don't exceed maximum length
	n = MIN(n, DISPLAY_OUTPUT_STR_MAX_LENGTH);

	for (uint16_t i = 0; i < n; ++i) {
		oled_display.write(str[i]);
	}

	return;
}

void display_write_state(sys_status_t* status){

	//Set cursor to the top again
	reset_cursor_status();

	snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "GPS:%2s, Acc:%2s \n",
														BOOL_TO_C(status->gps_com),
														BOOL_TO_C(status->accel_com));
	display_write_text(output_str);

	snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "Fix: %2s \n",
														BOOL_TO_C(status->gps_fix));
	display_write_text(output_str);

}

void display_write_local_coord(sys_status_t* status, position_t* pos){
	display_write_state(status);

	int16_t   val_x,  val_y;
	uint16_t pres_x, pres_y;

	if (status->gps_fix) {
		float_to_str(pos->x, &val_x, &pres_x);
		float_to_str(pos->y, &val_y, &pres_y);

		snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "x=%2d.%2d y=%2d.%2d \n", val_x, pres_x, val_y, pres_y);
	} else {
		snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "x=%5s y=%5s\n", EMPTY_VAL, EMPTY_VAL);
	}
	display_write_text(output_str);

}

void display_write_data(state_t* state){
	int16_t val;
	uint16_t pres;

	//Set cursor to the top again
	reset_cursor_data();

	display_write_text("Vertical: \n");
	float_to_str(state->pos, &val, &pres);
	snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "z=%3d.%2d \n", val, pres);
	display_write_text(output_str);


	float_to_str(state->vel, &val, &pres);
	snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "v=%3d.%2d \n", val, pres);
	display_write_text(output_str);

	float_to_str(state->acc, &val, &pres);
	snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "a=%3d.%2d \n", val, pres);
	display_write_text(output_str);
}

void display_test_char(){
	oled_display.drawChar(oled_display.width()/2, oled_display.height()/2, 'A', WHITE, BLACK, 3);
}

