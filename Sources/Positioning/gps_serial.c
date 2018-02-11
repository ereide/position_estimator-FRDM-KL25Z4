/*
 * uart.h
 *
 * This module provides an interface to simplify the serial communication for the GPS module
 *
 *  Created on: Jan 15, 2018
 *      Author: Eivind Roson Eide
 */

#include <string.h>

#include <gps_serial.h>
#include "fsl_uart_driver.h"
#include "fsl_tpm_driver.h"
#include "fsl_debug_console.h"
#include "fsl_clock_manager.h"

#define CR ('\r')		//Carriage return
#define LF ('\n')		//Line feed

#define GPS_UART_INSTANCE 		1u
#define GPS_TIMEOUT 			1000u
#define GPS_DEFAULT_BAUD_RATE 	9600u


static volatile uint8_t rxBuff[100] = {0};


// Most useful NMEA messages should be shorter than 120 chars
#define MAXLINELENGTH 120

// Double buffer: read one line in and leave one for the main program
volatile char line1[MAXLINELENGTH];
volatile char line2[MAXLINELENGTH];

// The index for filling the current line
volatile uint8_t lineidx=0;

// Pointers to the double buffers
volatile char *currentline;
volatile char *lastline;
volatile bool recvdflag;



static uart_state_t uartState;

static uart_user_config_t uartConfig = {
		.bitCountPerChar = kUart8BitsPerChar,
		.parityMode		 = kUartParityDisabled,
		.stopBitCount	 = kUartOneStopBit,
		.baudRate		 = GPS_DEFAULT_BAUD_RATE
};




static void serial_callback(uint32_t instance, void * uartState){
	char c = (char)(rxBuff[0]);

	if (c == '\n') {
		currentline[lineidx] = 0;


		if (currentline == line1) {
		  currentline = line2;
		  lastline = line1;
		} else {
		  currentline = line1;
		  lastline = line2;
		}

		lineidx = 0;
		recvdflag = true;

	}

	currentline[lineidx++] = c;
	if (lineidx >= MAXLINELENGTH){
		lineidx = MAXLINELENGTH-1;
	}

	return;


}

static void gps_read_init(void){
  recvdflag   = false;
  lineidx     = 0;
  currentline = line1;
  lastline    = line2;
}


static void install_callback(void){
	UART_DRV_InstallRxCallback(GPS_UART_INSTANCE, &serial_callback, (uint8_t*)rxBuff, &uartState, true);
}

void serial_delay(uint32_t milli){
	OSA_TimeDelay(milli);
}


void serial_init(uint32_t baud){

	CLOCK_SYS_EnableUartClock(GPS_UART_INSTANCE);

	uartConfig.baudRate = baud;

	UART_DRV_Init(GPS_UART_INSTANCE, &uartState, &uartConfig);
	PRINTF("- GPS: Serial communication setup complete. \n\r");


	gps_read_init();

	install_callback();
	PRINTF("- GPS: Interrupt setup complete. \n\r");
}


void print_last_NMEA(void){
	PRINTF(lastline);
	PRINTF("\n\r");
}


void serial_println(const char *str){
	//Determine size of array
	size_t n = strlen(str);

	//We want to send
	char endline[2] = {CR, LF};

	//sends characters
	UART_DRV_SendDataBlocking(GPS_UART_INSTANCE, (uint8_t*)str, n, GPS_TIMEOUT);

	//terminates the line
	UART_DRV_SendDataBlocking(GPS_UART_INSTANCE, (uint8_t*)endline, 2, GPS_TIMEOUT);
}
