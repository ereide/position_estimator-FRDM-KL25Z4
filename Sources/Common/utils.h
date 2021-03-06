/*
 * utils.h
 *
 *  Created on: Feb 11, 2018
 *      Author: eivind
 */

#ifndef UTILS_H_
#define UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MAX(a,b) (((a)>(b))?(a):(b))



void float_to_str(float f, char* sign, uint16_t* val, uint16_t* pres);



#ifdef __cplusplus
}
#endif

#endif /* UTILS_H_ */
