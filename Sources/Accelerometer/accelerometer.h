/*
 * accelerometer.h
 *
 *  Created on: Feb 10, 2018
 *      Author: eivind
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <stdbool.h>


#include "types.h"

void init_accel(void);

bool get_accel_data(acceleration_t *acc);


#endif /* ACCELEROMETER_H_ */
