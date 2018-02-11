/*
 * gps.h
 *
 *  Created on: Dec 16, 2017
 *      Author: Eivind Roson Eide (c)
 */

#ifndef GPS_H
#define GPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"



void init_gps(sys_status_t* status);

void gps_setzero(void);

bool gps_read(position_t* pos, velocity_t* vel, sys_status_t* status);



#ifdef __cplusplus
}
#endif

#endif /* GPS_H */
