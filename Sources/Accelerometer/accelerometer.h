/*
 * accelerometer.h
 *
 *	API for accessing the accelerometer data
 *
 *  Created on: Feb 10, 2018
 *      Author: Eivind Roson Eide
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <stdbool.h>


#include "types.h"

void init_accel(sys_status_t* status);

//Extracts pure acceleration data without taking into account gravity
bool get_accel_data(acceleration_t *acc);

//Extracts corrected acceleration data assuming no acceleration in x and y direction
bool get_z_accel(acceleration_t* acc);


#endif /* ACCELEROMETER_H_ */
