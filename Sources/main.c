/*
 * Copyright (c) 2018, Eivind Roson Eide
 *
 * Parts of the code is borrowed from Freescale semiconductors
 */

/*
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

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
#include <string.h>
// SDK Included Files
#include "board.h"
#include "fsl_tpm_driver.h"
#include "fsl_debug_console.h"

#include "accel.h"
#include "linear_predictor_small.h"
#include "gps.h"
#include "display.h"

///////////////////////////////////////////////////////////////////////////////
//  Defines
///////////////////////////////////////////////////////////////////////////////

#define GRAVITY_ACCELERATION 				9.81		// m/s^2
#define ACCEL_MMA8451_4G_OFFSET 			8192		// Half the range = 2^13 = 1<<14
#define ACCEL_MMA8451_4G_COUNTS_PER_G 		(2048*4)	// Number of counts for each G offset by 2 bits.
#define ACCEL_MMA8451_4G_ACCURACY_COUNTS 	200  		// TODO Check


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

static void LED_toggle_master(void)
{
    GPIO_DRV_TogglePinOutput(kGpioLED1);
}


void init_system(void) {
	PRINTF("\n\r");

    // Initialize standard SDK demo application pins.
    hardware_init();

    // Accel device driver utilizes the OSA, so initialize it.
    OSA_Init();

    OSA_TimeDelay(5);

    PRINTF("System initialization complete!\r\n\r\n");

}

void accel_setup(	accel_dev_t *accDev,
	    			accel_dev_interface_t *accDevice,
					accel_i2c_interface_t *i2cInterface){

    // Register callback func for I2C
    i2cInterface->i2c_init       =  I2C_DRV_MasterInit;
    i2cInterface->i2c_read       =  I2C_DRV_MasterReceiveDataBlocking;
    i2cInterface->i2c_write      =  I2C_DRV_MasterSendDataBlocking;

    accDev->i2c      = i2cInterface;
    accDev->accel    = accDevice;

    accDev->slave.baudRate_kbps  = BOARD_ACCEL_BAUDRATE;
    accDev->slave.address        = BOARD_ACCEL_ADDR;
    accDev->bus                  = BOARD_ACCEL_I2C_INSTANCE;
}

float accel_conversion(int16_t data){
	return data*(float)GRAVITY_ACCELERATION/(float)ACCEL_MMA8451_4G_COUNTS_PER_G;
}

void get_accel_data(const accel_dev_t* accDev, float *xData, float *yData, float *zData){
    // Get new accelerometer data.
    accel_sensor_data_t accelData;

    accDev->accel->accel_read_sensor_data(accDev, &accelData);

    int16_t x, y, z;

    // Get the X and Y data from the sensor data structure.fxos_data
    x = (int16_t)((accelData.data.accelXMSB << 8) | accelData.data.accelXLSB);
    y = (int16_t)((accelData.data.accelYMSB << 8) | accelData.data.accelYLSB);
    z = (int16_t)((accelData.data.accelZMSB << 8) | accelData.data.accelZLSB);

    PRINTF("RAW: 		x= %d 	 y = %d 	  z = %d\r\n", x, y, z);


    *xData = accel_conversion(x);
    *yData = accel_conversion(y);
    *zData = accel_conversion(z) - 1; //Removing gravity
}


/*!
 * @brief Main demo function.
 */
int main (void)
{

	//Initialize system drivers
	init_system();

	//Initialize OLED screen
	init_display();

	PRINTF("OLED SSD1331 setup complete \n\r");


	OSA_TimeDelay(500);

	draw_something();

	PRINTF("Init complete \n\r");

	while(1){
		OSA_TimeDelay(1000);
		draw_something();
		draw_something_else();


	}




	//gps_init();


	/*

	char c;

	while(1){
		//PRINTF("Something \n\r");
		c = gps_read();
		PRINTF("%X", c);
	}
	*/

	/*
	//Setup and initialize accelerometer
	accel_dev_t accDev;
    accel_dev_interface_t accDevice;
    accel_i2c_interface_t i2cInterface;

    accel_setup(&accDev, &accDevice, &i2cInterface);

    // Initialize the Accel.
    accel_init(&accDev);

    PRINTF("Accelerometer setup complete!\r\n\r\n");


	float xData, yData, zData;
	state_t state;

	//Init kalman filters
	init_filters(MODEL_ACCEL_VAR, OBSERVATION_POS_VAR, OBSERVATION_ACCEL_VAR);

    // Main loop.  Get sensor data and update duty cycle for the TPM timer.
    while(1)
    {
        PRINTF("Reading data\r\n");

        // Wait 5 ms in between samples (accelerometer updates at 200Hz).
        OSA_TimeDelay(100);


        get_accel_data(&accDev, &xData, &yData, &zData);

        predict();
        update_accel(xData, yData, zData);
        update_pos(0, 0, 0);

        getzState(&state);

        // Print out the accelerometer data.
        PRINTF("Converted:	x= %de-3 y = %de-3 z = %de-3\r\n", (int16_t)(xData*1000), (int16_t)(yData*1000), (int16_t)(zData*1000));

        PRINTF("State    :	z= %de-3 v = %de-3 a = %de-3\r\n", (int16_t)(state.pos*1000), (int16_t)(state.vel*1000), (int16_t)(state.acc*1000));
    }

	*/

}
