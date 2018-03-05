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

#define SIGN(X)  ((X) < 0 ? '-' : '+')

void float_to_str(float f, char* sign, uint16_t* val, uint16_t* pres){
	float temp;
	temp  = trunc(f);
	*val  = (uint16_t)abs(temp);
	temp  = f - temp;
	*pres = (uint16_t)(trunc(abs(temp*100)));
	*sign = SIGN(f);
}
