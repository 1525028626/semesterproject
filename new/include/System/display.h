#ifndef DISPLAY_H
#define DISPLAY_H
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "FT6336.h"
#include "GUI_Paint.h"
#include <Arduino.h>
#include <LVGL.h>
#include "ui/ui.h"
#define rgb565_gray4 0x0000
#define rgb565_gray3 0x8410
#define rgb565_gray2 0xC618
#define rgb565_gray1 0xffff
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_color_t buf[EPD_WIDTH * EPD_HEIGHT];

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void lvgl_set_init(void);
void refresh_screen(void);

bool lvgl_is_display_updated(void);

#endif