/*
 *  oled_spi.h
 *
 *	Low level driver for communicating over SPI
 *
 *  Created on: Feb 8, 2018
 *      Author: Eivind Roson Eide
 */

#ifndef DISPLAY_OLED_SPI_H_
#define DISPLAY_OLED_SPI_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

//Initialize OLED screen connection
void oled_spi_init(void);

//Write Command interface
void oled_writeCommand(uint8_t commandByte);

//Write Data interface
void oled_writeData(uint8_t dataByte);

void oled_delay(uint32_t milli);

#ifdef __cplusplus
}
#endif


#endif /* DISPLAY_OLED_SPI_H_ */
