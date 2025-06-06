#include "System/display.h"
#include "lvgl.h"
u8_t gray_buffer[EPD_WIDTH * EPD_HEIGHT/4];
// 在文件顶部添加全局变量
static bool display_updated = false;

void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    // 添加变化检测标志
    bool has_changes = false;
    
    for(int i = area->y1; i <= area->y2; i ++){
        for(int j = area->x1; j <= area->x2; j ++){
            uint16_t current_color = Paint_GetPixel(j, i);
            uint16_t new_color = color_p[(j-area->x1) + (i-area->y1)*w].full;
            if(current_color != new_color){
                if(new_color == 0xffff){
                    Paint_SetPixel(j,  i, WHITE); 
                }
                // else if(new_color >= rgb565_gray2){
                //     Paint_SetPixel(j,  i, GRAY2);
                // }else if (new_color >= rgb565_gray3){
                //     Paint_SetPixel(j,  i, GRAY3);
                // }
                else{
                    Paint_SetPixel(j,  i, BLACK);
                }
                has_changes = true;
            }                  
        }
    }
    
    if(has_changes) display_updated = true;
    lv_disp_flush_ready(disp_drv);
}

// 添加获取更新状态的函数
bool lvgl_is_display_updated() {
    bool ret = display_updated;
    display_updated = false; // 读取后自动重置标志
    return ret;
}

void lvgl_set_init(void) { 

    lv_init();

    
    EPD_HW_Init();
    EPD_WhiteScreen_White();
    delay(500);

    Paint_NewImage(gray_buffer, EPD_WIDTH, EPD_HEIGHT,270,WHITE);
    Paint_SetScale(2);
    Paint_SetMirroring(1);
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, EPD_WIDTH * EPD_HEIGHT);
    lv_disp_drv_init(&disp_drv);    
    disp_drv.hor_res = EPD_WIDTH;
    disp_drv.ver_res = EPD_HEIGHT;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.flush_cb = disp_flush;
    lv_disp_drv_register(&disp_drv);
    lv_disp_set_rotation(lv_disp_get_default(), LV_DISP_ROT_270);
    
    
}
void refresh_screen() { 
    static uint8_t refresh_count = 0;  // 添加静态计数器
    
    // 每5次刷新执行一次全刷
    if (++refresh_count % 5 == 0) {
        EPD_HW_Init();      // 全刷初始化
        EPD_WhiteScreen_ALL(gray_buffer);  // 执行全刷
    } else { 
        EPD_HW_Init_Fast();
        EPD_WhiteScreen_ALL_Fast(gray_buffer);        // 执行快刷（假设EPD_Update是快刷函数）
    }
}