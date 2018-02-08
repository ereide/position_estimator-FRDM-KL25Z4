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


#define CR '\r'		//Carriage return
#define LF '\n'		//Line feed

#define GPS_UART_INSTANCE 1u
#define GPS_TIMEOUT 1000u
#define GPS_DEFAULT_BAUD_RATE 9600



static uart_state_t uartState;

static uart_user_config_t uartConfig = {
		.bitCountPerChar = kUart8BitsPerChar,
		.parityMode		 = kUartParityDisabled,
		.stopBitCount	 = kUartOneStopBit,
		.baudRate		 = GPS_DEFAULT_BAUD_RATE
};


void serial_delay(uint32_t milli){
	OSA_TimeDelay(milli);
}

void serial_init(uint32_t baud){
	uartConfig.baudRate = baud;

	UART_DRV_Init(GPS_UART_INSTANCE, &uartState, &uartConfig);
}


void serial_println(const char *str){
	//Determine size of array
	size_t n = strlen(str);

	//We want to send
	char endline[2] = {CR, LF};

	PRINTF("n = %d \n\r", n);

	//sends characters
	//UART_DRV_SendDataBlocking(GPS_UART_INSTANCE, (uint8_t*)str, n, GPS_TIMEOUT);

	//terminates the line
	//UART_DRV_SendDataBlocking(GPS_UART_INSTANCE, (uint8_t*)endline, 2, GPS_TIMEOUT);
}


bool serial_read(char *c){

	uart_status_t state = UART_DRV_ReceiveDataBlocking(GPS_UART_INSTANCE, (uint8_t*)c, 1, GPS_TIMEOUT);


	if (state !=  kStatus_UART_Success) {
		return false;
	}

	else {
		return true;
	}
}

