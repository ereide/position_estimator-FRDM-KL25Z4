/*
 * accelerometer.c
 *
 *  Created on: Feb 10, 2018
 *      Author: Eivind Roson Eide
 *
 *  Wrapper for the general accel library
 */

#include "accelerometer.h"

#include "accel.h"
#include "board.h"

#include "fsl_debug_console.h"

///////////////////////////////////////////////////////////////////////////////
//  Defines
///////////////////////////////////////////////////////////////////////////////

#define GRAVITY_ACCELERATION 				9.81		// m/s^2
#define ACCEL_MMA8451_4G_OFFSET 			8192		// Half the range = 2^13 = 1<<14
#define ACCEL_MMA8451_4G_COUNTS_PER_G 		(2048*4)	// Number of counts for each G offset by 2 bits.
#define ACCEL_MMA8451_4G_ACCURACY_COUNTS 	200  		// TODO Check

#define ACCEL_SUCCESS_VALUE					0


//Setup and initialize accelerometer
static accel_dev_t accDev;
static accel_dev_interface_t accDevice;
static accel_i2c_interface_t i2cInterface;
static accel_sensor_data_t accelData;


static float accel_conversion(int16_t data){
	return data*(float)GRAVITY_ACCELERATION/(float)ACCEL_MMA8451_4G_COUNTS_PER_G;
}

void init_accel(void){
    // Register callback func for I2C
    i2cInterface.i2c_init       =  I2C_DRV_MasterInit;
    i2cInterface.i2c_read       =  I2C_DRV_MasterReceiveDataBlocking;
    i2cInterface.i2c_write      =  I2C_DRV_MasterSendDataBlocking;

    accDev.i2c      			= &i2cInterface;
    accDev.accel    			= &accDevice;

    accDev.slave.baudRate_kbps  = BOARD_ACCEL_BAUDRATE;
    accDev.slave.address        = BOARD_ACCEL_ADDR;
    accDev.bus                  = BOARD_ACCEL_I2C_INSTANCE;

    // Initialize the Accelerometer.
    accel_init(&accDev);
}



bool get_accel_data(acceleration_t *acc){
    // Get new accelerometer data.

    uint8_t retval =  accDev.accel->accel_read_sensor_data(&accDev, &accelData);

    //TODO: seems to be an error in the documentation
    if (retval == 1) {
        int16_t x, y, z;

        // Get the X and Y data from the sensor data structure.fxos_data
        x = (int16_t)((accelData.data.accelXMSB << 8) | accelData.data.accelXLSB);
        y = (int16_t)((accelData.data.accelYMSB << 8) | accelData.data.accelYLSB);
        z = (int16_t)((accelData.data.accelZMSB << 8) | accelData.data.accelZLSB);

        PRINTF("RAW: 		x= %d 	 y = %d 	  z = %d\r\n", x, y, z);

        //TODO: Check these

        acc->x_acc = accel_conversion(x);
        acc->y_acc = accel_conversion(y);
        acc->z_acc = accel_conversion(z) - 1; //Removing gravity

        return true;
	}

    else{
    	return false;
    }



}
