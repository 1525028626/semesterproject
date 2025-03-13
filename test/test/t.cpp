/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include  "head.h"
#include "lvgl/lvgl.h"
#include "lvgl_helpers.h"

// 定义任务句柄
TaskHandle_t epdTaskHandle = NULL;

// 定义LVGL显示缓冲区
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

// 显示刷新回调函数，用于将LVGL的绘制内容更新到墨水屏
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    int32_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            // 这里需要根据墨水屏的驱动接口将像素数据写入到指定位置
            // 示例中简单假设存在一个函数将像素写入墨水屏
            // 你需要根据实际的墨水屏驱动进行修改
            // set_pixel(x, y, color_p->full);
            color_p++;
        }
    }
    lv_disp_flush_ready(disp_drv);
}

/* Entry point ----------------------------------------------------------------*/
void epdTask(void *pvParameters) {
    Serial.begin(115200);
    printf("EPD_2IN7_test Demo\r\n");
    DEV_Module_Init();

    // 初始化LVGL
    lv_init();

    // 初始化显示缓冲区
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    // 初始化显示驱动
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = EPD_2IN7_WIDTH;
    disp_drv.ver_res = EPD_2IN7_HEIGHT;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN7_Init();
    Serial.print("11");
    EPD_2IN7_Clear();
    DEV_Delay_ms(500);

    // 创建一个屏幕对象
    lv_obj_t *scr = lv_disp_get_scr_act(NULL);

    // 创建一个标签对象并设置文本
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "Hello, E-Ink Display with LVGL!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // 主循环，用于更新LVGL界面
    while(1) {
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }

    printf("Goto Sleep...\r\n");
    EPD_2IN7_Init();
    EPD_2IN7_Clear();
    EPD_2IN7_Sleep();
}

void setup() {
    // 创建任务
    xTaskCreate(
        epdTask,        // 任务函数
        "epdTask",      // 任务名称
        4096,           // 任务堆栈大小
        NULL,           // 任务参数
        1,              // 任务优先级
        &epdTaskHandle  // 任务句柄
    ); 
}

/* The main loop -------------------------------------------------------------*/
void loop() {
    // FreeRTOS 会自动调度任务，主循环可以留空
}