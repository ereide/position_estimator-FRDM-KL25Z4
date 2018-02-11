/*
 * Copyright (c) 2018, Eivind Roson Eide.
 *
 *
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "pin_mux.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"

static void tpm_mux_select(void){
	// configure tpm pins
	PORT_HAL_SetMuxMode(PORTB,18u,kPortMuxAlt3);
	PORT_HAL_SetMuxMode(PORTB,19u,kPortMuxAlt3);
}

static void spi_oled_mux_select(void){
	// Setup SPI1 pins for OLED
	/*	KL25_SPI_MISO	--> PTD3	(ALT2)		*/
	PORT_HAL_SetMuxMode(PORTD, 3u,  kPortMuxAlt2);

	/*	KL25_SPI_MOSI	--> PTD2	(ALT2)		*/
	PORT_HAL_SetMuxMode(PORTD, 2u,  kPortMuxAlt2);

	/*	KL25_SPI_SCK	--> PTD1	(ALT2)		*/
	PORT_HAL_SetMuxMode(PORTD, 1u,  kPortMuxAlt2);

	// Setup GPIO pins for OLED
	PORT_HAL_SetMuxMode(PORTD, 0u,  kPortMuxAsGpio); // OCS
	PORT_HAL_SetMuxMode(PORTD, 5u,  kPortMuxAsGpio); // DC
	PORT_HAL_SetMuxMode(PORTA, 13u, kPortMuxAsGpio); // RST
}

static void i2c_accel_mux_select(void){
	// configure i2c pins
	//Sets the mux mode for the uart pins PE0 and PE1
	/* PORTE_PCR24 */
	PORT_HAL_SetMuxMode(PORTE,24u,kPortMuxAlt5);
	/* PORTE_PCR25 */
	PORT_HAL_SetMuxMode(PORTE,25u,kPortMuxAlt5);
}

static void uart_gps_mux_select(void){
    /* PORTE_PCR0 */
    PORT_HAL_SetMuxMode(PORTE,0u, kPortMuxAlt3);
    /* PORTE_PCR1 */
    PORT_HAL_SetMuxMode(PORTE,1u, kPortMuxAlt3);
}

static void gpio_led_mux_select(void){
	PORT_HAL_SetMuxMode(PORTB, 18u, kPortMuxAsGpio);  // RED_LED
	PORT_HAL_SetMuxMode(PORTB, 19u, kPortMuxAsGpio);  // GREEN_LED
	PORT_HAL_SetMuxMode(PORTD, 1u,  kPortMuxAsGpio);  // BLUE_LED
}


static void enable_clock(void){
	/* enable clock for PORTs */
	CLOCK_SYS_EnablePortClock(PORTA_IDX);
	CLOCK_SYS_EnablePortClock(PORTB_IDX);
	CLOCK_SYS_EnablePortClock(PORTC_IDX);
	CLOCK_SYS_EnablePortClock(PORTD_IDX);
	CLOCK_SYS_EnablePortClock(PORTE_IDX);
}

void hardware_init(void) {

	enable_clock();

	//Set GPIO
	gpio_led_mux_select();

	//Set TPM pins
	tpm_mux_select();

	//Set UART pins for communicating with GPS
	uart_gps_mux_select();

	//Set I2C pins for communicating with accelerometer
	i2c_accel_mux_select();

	//Set SPI pins for communicating with OLED screen
	spi_oled_mux_select();

	/* Select the clock source for the TPM counter */
	CLOCK_SYS_SetTpmSrc(2u, kClockTpmSrcPllFllSel);

	/* Init board clock */
	BOARD_ClockInit();
	dbg_uart_init();

    GPIO_DRV_Init(0, outputPins);
}

