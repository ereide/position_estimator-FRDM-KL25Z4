/*
 * utils.c
 *
 *  Created on: Feb 11, 2018
 *      Author: eivind
 */

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void float_to_str(float f, int16_t* val, uint16_t* pres){
	float temp;
	*val = trunc(f);
	temp = f - (*val);
	*pres = (uint16_t)(trunc(abs(temp*100)));
}
