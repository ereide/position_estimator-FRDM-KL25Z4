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
#define DISPLAY_TEXT_WRAP				false


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
	int16_t y0 = 0; //oled_display.height() / 16;
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
	//oled_display.setFont();

	oled_display.setCursor(0, 0);
	oled_display.setTextColor(DISPLAY_TEXT_COLOR, DISPLAY_BACKGROUND_COLOR);
	oled_display.setTextSize(DISPLAY_TEXT_SIZE);
	oled_display.setTextWrap(DISPLAY_TEXT_WRAP);
}

static void configure_cursor(void){
	reset_cursor();

	//Finds the location of the fourth line
	oled_display.write('\n');
	oled_display.write('\n');
	oled_display.write('\n');
	oled_display.write('\n');

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

	configure_cursor();

}

//Interface for writing a string to the display
void display_write_text(char* str){

	size_t n = strlen(str);

	//Don't exceed maximum length
	n = MIN(n, DISPLAY_OUTPUT_STR_MAX_LENGTH);

	for (uint16_t i = 0; i < n; ++i) {
		oled_display.write(str[i]);
	}

	return;
}


//Function for displaying the latest status data
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

//Function for displaying latest position data
void display_write_local_coord(sys_status_t* status, position_t* pos){
	display_write_state(status);

	int16_t   val_x,  val_y;
	uint16_t  pres_x, pres_y;
	char      sign_x, sign_y;

	if (status->gps_fix) {
		float_to_str(pos->x, &sign_x, &val_x, &pres_x);
		float_to_str(pos->y, &sign_y, &val_y, &pres_y);

		snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "x=%c%3d.%2d y=&c%3d.%2d \n", sign_x, val_x, pres_x, sign_y, val_y, pres_y);
	} else {
		snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "x=%7s y=%7s\n", EMPTY_VAL, EMPTY_VAL);
	}
	display_write_text(output_str);

}

//Function for displaying the latest altitude data
void display_write_data(state_t* state){
	int16_t  val;
	uint16_t pres;
	char 	 sign;

	//Set cursor to the top again
	reset_cursor_data();

	display_write_text("Vertical: \n");
	float_to_str(state->pos, &sign, &val, &pres);
	snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "z=%c%3d.%2d \n", sign, val, pres);
	display_write_text(output_str);


	float_to_str(state->vel, &sign, &val, &pres);
	snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "v=%c%3d.%2d \n", sign, val, pres);
	display_write_text(output_str);

	float_to_str(state->acc, &sign, &val, &pres);
	snprintf(output_str, DISPLAY_OUTPUT_STR_MAX_LENGTH, "a=%c%3d.%2d \n", sign, val, pres);
	display_write_text(output_str);
}


//To test that the module is working
void display_test_char(){
	oled_display.drawChar(oled_display.width()/2, oled_display.height()/2, 'A', WHITE, BLACK, 3);
}

void display_test_write(){
	oled_display.write('H');
	oled_display.write('E');
	oled_display.write('Y');
}
