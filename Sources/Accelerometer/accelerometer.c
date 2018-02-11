/*
 * accelerometer.c
 *
 *  Created on: Feb 10, 2018
 *      Author: Eivind Roson Eide
 *
 */

#include "accelerometer.h"

#include "types.h"

#include "accel.h"
#include "board.h"
#include "utils.h"
#include "math.h"

#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"

///////////////////////////////////////////////////////////////////////////////
//  Defines
///////////////////////////////////////////////////////////////////////////////

#define GRAVITY_ACCELERATION 				9.81		// m/s^2
#define ACCEL_MMA8451_4G_OFFSET 			8192		// Half the range = 2^13 = 1<<14
#define ACCEL_MMA8451_4G_COUNTS_PER_G 		(2048*4)	// Number of counts for each G offset by 2 bits.
#define ACCEL_MMA8451_4G_ACCURACY_COUNTS 	200  		// TODO Check

#define ACCEL_SUCCESS_VALUE					0
#define ACCEL_AVERAGE_N						10
#define ACCEL_UPDATE_RATE_MS				10			// Accelerometer updates at 200Hz => T = 5ms

//Setup and initialize accelerometer
static accel_dev_t accDev;
static accel_dev_interface_t accDevice;
static accel_i2c_interface_t i2cInterface;
static accel_sensor_data_t accelData;

static float gravity_magnitude;

static float accel_conversion(int16_t data){
	return data*(float)GRAVITY_ACCELERATION/(float)ACCEL_MMA8451_4G_COUNTS_PER_G;
}

static float accel_magnitude(acceleration_t* acc){
	return sqrtf(acc->x_acc * acc->x_acc +
				 acc->y_acc * acc->y_acc +
				 acc->z_acc * acc->z_acc);
}

static void init_accel_com(void){
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
	PRINTF("- ACCEL: I2C communication setup complete \n\r");

}

void accel_set_zero(void){
	acceleration_t temp_accel;
	for (uint16_t i = 0; i < ACCEL_AVERAGE_N; ++i) {
		OSA_TimeDelay(ACCEL_UPDATE_RATE_MS);
		get_accel_data(&temp_accel);
		gravity_magnitude += accel_magnitude(&temp_accel);
	}

	gravity_magnitude = gravity_magnitude/ACCEL_AVERAGE_N;

	int16_t val;
	uint16_t pres;
	float_to_str(gravity_magnitude, &val, &pres);

	PRINTF("- ACCEL: Gravity constant calculated to %d.%2d \n\r", val, pres);
}


void init_accel(sys_status_t* status){
	//setup communication
	init_accel_com();

	status->accel_com = true;

	//Removes the zero offset to combat angle error
    accel_set_zero();
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

        //PRINTF("RAW: 		x= %d 	 y = %d 	  z = %d\r\n", x, y, z);

        //TODO: Check these

        acc->x_acc = accel_conversion(x);
        acc->y_acc = accel_conversion(y);
        acc->z_acc = accel_conversion(z);

        return true;
	}

    else{
    	return false;
    }
}

/*
 * get_z_accel(acceleration_t* acc)
 *
 * Corrects for orientation error by assuming x and y acceleration is 0.
 *
 * Returns true if reading was succesful
 */
bool get_z_accel(acceleration_t* acc){
	bool retVal = get_accel_data(acc);

	float mag   = accel_magnitude(acc);

	acc->x_acc = 0;
	acc->y_acc = 0;
	acc->z_acc = mag - gravity_magnitude; //Assumes the acceleration in x and y direction is negligible

	return retVal;
}

