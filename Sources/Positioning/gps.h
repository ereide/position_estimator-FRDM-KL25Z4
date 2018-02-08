#ifndef GPS_H
#define GPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ctype.h"

struct position {
	float x;
	float y;
	float z;
};

void gps_init(void);

void gps_setzero(void);

char gps_read(void);



#ifdef __cplusplus
}
#endif

#endif /* GPS_H */
