#ifndef GPS_H
#define GPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"



void init_gps(void);

void gps_setzero(void);

bool gps_read(position_t* pos);



#ifdef __cplusplus
}
#endif

#endif /* GPS_H */
