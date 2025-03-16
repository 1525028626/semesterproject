// // ... 头文件部分保持不变 ...
// #include  "head.h"
// #include <lvgl.h>

// // 修改显示驱动函数
// static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
// {   
//     EPD_2IN7_Init_4Gray();
//     EPD_2IN7_Clear();
//     // 全屏转换（即使只更新部分区域）
//     for(int y = 0; y < EPD_2IN7_HEIGHT; y++) {
//         for(int x = 0; x < EPD_2IN7_WIDTH; x++) {
//             lv_color_t c = lv_buf[x + y * EPD_2IN7_WIDTH];
//             uint8_t gray = (c.ch.red * 5 + c.ch.green * 6 + c.ch.blue * 5) / 64;
//             uint32_t pos = (x + y * EPD_2IN7_WIDTH) / 4;
//             epd_buffer[pos] = (epd_buffer[pos] & ~(0xC0 >> ((x % 4)*2))) | (gray << (6 - (x % 4)*2));
//         }
//     }
    
//     // 强制全屏刷新
//     EPD_2IN7_4GrayDisplay(epd_buffer);
//     lv_disp_flush_ready(disp_drv);
// }

// void epdTask(void *pvParameters) {
//     // ... 硬件初始化保持不变 ...
//     EPD_2IN7_Init_4Gray();
//     EPD_2IN7_Clear();
//     DEV_Delay_ms(500);

//     // 修改LVGL初始化配置
//     lv_init();
//     lv_disp_draw_buf_init(&draw_buf, lv_buf, NULL, EPD_2IN7_WIDTH * EPD_2IN7_HEIGHT);

//     static lv_disp_drv_t disp_drv;
//     lv_disp_drv_init(&disp_drv);
//     disp_drv.draw_buf = &draw_buf;
//     disp_drv.flush_cb = disp_flush;
//     disp_drv.hor_res = EPD_2IN7_WIDTH;
//     disp_drv.ver_res = EPD_2IN7_HEIGHT;
//     lv_disp_drv_register(&disp_drv);

//     // 创建示例UI（可替换为实际界面）
//     lv_obj_t * slider = lv_slider_create(lv_scr_act());
//     lv_slider_set_value(slider, 70, LV_ANIM_OFF);
//     lv_obj_center(slider);


//     // 添加屏幕保护机制
//     static uint32_t last_refresh = 0;
//     while(1) {
        
//         // 限制刷新频率（每分钟最多10次）
//         if(xTaskGetTickCount() - last_refresh > 6000) {
//             lv_timer_handler();
//             last_refresh = xTaskGetTickCount();
//         }
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
//     // ... 清理代码保持不变 ...
// }