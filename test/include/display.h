#ifndef __display_H_
#define __display_H_

#include <lvgl.h>
#include <head.h>

static lv_disp_draw_buf_t draw_buf;
static lv_color_t lv_buf[EPD_2IN7_WIDTH * EPD_2IN7_HEIGHT]; // 全屏缓冲
static UBYTE *epd_buffer; 

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void epdTask(void *pvParameters);
#endif