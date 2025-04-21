#include "display.h"
UBYTE gray_buffer[EPD_2IN7_WIDTH * EPD_2IN7_HEIGHT/4];
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_color_t buf[EPD_2IN7_WIDTH * EPD_2IN7_HEIGHT];
void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    // Serial.printf("x1:%d, y1:%d, x2:%d, y2:%d\n", area->x1, area->y1, area->x2, area->y2);
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    // for(int i = area->y1; i <= area->y2; i ++){
    //     for(int j = area->x1; j <= area->x2; j ++){
    //         if(color_p[(j-area->x1) + (i-area->y1)*w].full == 0xffff) Paint_SetPixel(j, i, WHITE);
    //         else Paint_SetPixel(j, i, BLACK);                  
    //     }
    // }
    for(int i = area->y1; i <= area->y2; i ++){
        for(int j = area->x1; j <= area->x2; j ++){
            lv_color_t c = color_p[(j-area->x1) + (i-area->y1)*w];
            
            // RGB565转灰度（0-255）
            uint8_t gray = ((c.ch.red * 38) + (c.ch.green * 75) + (c.ch.blue * 15)) >> 7;
            
            // 直接映射到0-3（0=白，3=黑）
            uint8_t level = gray >> 6; // 等价于 gray / 64
            Paint_SetPixel(j, i, 3 - level); // 反转映射
        }
    }
    lv_disp_flush_ready(disp_drv);
}
void lvgl_set_init(void) { 

    lv_init();
    EPD_2IN7_Init_4Gray();
    EPD_2IN7_Clear();
    DEV_Delay_ms(500);

    Paint_NewImage(gray_buffer, EPD_2IN7_WIDTH, EPD_2IN7_HEIGHT, 0,WHITE);
    Paint_SetScale(4);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, EPD_2IN7_WIDTH * EPD_2IN7_HEIGHT);
    lv_disp_drv_init(&disp_drv);    
    disp_drv.hor_res = EPD_2IN7_WIDTH;
    disp_drv.ver_res = EPD_2IN7_HEIGHT;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = disp_flush;
    lv_disp_drv_register(&disp_drv);

}
void refresh_screen() { 
    EPD_2IN7_4GrayDisplay(gray_buffer);
}