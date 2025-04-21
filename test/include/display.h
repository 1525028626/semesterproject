#ifndef DISPLAY_H
#define DISPLAY_H

#include "head.h"


void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void lvgl_set_init(void);
void refresh_screen(void);

extern UBYTE gray_buffer[EPD_2IN7_WIDTH * EPD_2IN7_HEIGHT/4];
#endif