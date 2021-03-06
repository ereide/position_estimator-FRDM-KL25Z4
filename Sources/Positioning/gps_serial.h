/*
 * serial.h
 *
 * This module provides an interface to simplify the serial communication for the GPS module
 *
 *  Created on: Jan 15, 2018
 *      Author: Eivind Roson Eide
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


// pointer to the last line
extern volatile char *lastline;

// flag telling us if we have received a new message
extern volatile bool recvdflag;


//Inializes the serial interface
void serial_init(uint32_t baud);

//Wraps OSA_TimeDalay
void serial_delay(uint32_t milli);

//Reads a single character. Returns false if it times out
bool serial_read(char *c);

//Sends a the chars stored in str[] and ends it with a newline
void serial_println(const char *str);

void print_last_NMEA(void);


#ifdef __cplusplus
}
#endif


#endif /* SERIAL_H */
