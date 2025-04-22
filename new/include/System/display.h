#ifndef DISPLAY_H
#define DISPLAY_H
#include "EPD.h"
#include "GUI_Paint.h"
#include <LVGL.h>


static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_color_t buf[EPD_2IN7_WIDTH * EPD_2IN7_HEIGHT];

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void lvgl_set_init(void);
void refresh_screen(void);



#endif