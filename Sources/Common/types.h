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

#include "stdbool.h"

typedef struct {
	float x;	//East
	float y;	//North
	float z;	//Up
} position_t;

typedef struct {
	float x_vel; //East
	float y_vel; //North
	float z_vel; //Up
} velocity_t;


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


typedef struct {
	bool gps_com;
	bool gps_fix;
	bool accel_com;
	bool display_com;
} sys_status_t;

#ifdef __cplusplus
}
#endif

#endif /* SOURCES_TYPES_H_ */
