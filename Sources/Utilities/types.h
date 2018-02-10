/*
 * types.h
 *
 *  Created on: Feb 10, 2018
 *      Author: Eivind Roson Eide
 */

#ifndef TYPES_H_
#define TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	float x;
	float y;
	float z;
} position_t;


typedef struct {
	float x_acc;
	float y_acc;
	float z_acc;
} acceleration_t;


typedef struct {
	float pos;
	float vel;
	float acc;
} state_t;


#ifdef __cplusplus
}
#endif

#endif /* SOURCES_TYPES_H_ */
