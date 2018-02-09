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

#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

void init_display(void);

void draw_something(void);
void draw_something_else(void);


#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_H_ */
