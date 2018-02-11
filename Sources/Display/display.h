/*
 * display.h
 *
 *  Created on: Feb 8, 2018
 *      Author: Eivind Roson Eide
 */


#ifndef DISPLAY_H_
#define DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

#include "types.h"


////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////

#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF



///////////////////////////////////////////////////////////////////////////////
//  Function Prototypes
///////////////////////////////////////////////////////////////////////////////

void init_display(sys_status_t* status);
void display_empty_screen(void);
void display_write_text(char* str);
void display_write_data(state_t* state);
void display_write_local_coord(sys_status_t* status, position_t* pos);
void display_write_state(sys_status_t* status);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_H_ */
