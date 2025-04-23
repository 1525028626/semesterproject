#include "System/Conf.h"
TaskHandle_t Screen_Handle;
TaskHandle_t weather_Handle;
TaskHandle_t wordbook_Handle; 

void  SystemInit() {
    DEV_Module_Init();
    lvgl_set_init();
    ui_init();

    Devices_Init();
    Applications_Init();
}

void Devices_Init(void) {
    xTaskCreatePinnedToCore(Device_Screen,      "Dev_Screen",       10000,  NULL, 1, &Screen_Handle,    0); 
}
void Applications_Init(void) {
    xTaskCreatePinnedToCore(weather_task    , "weather_task",       10000,  NULL, 2, &weather_Handle    ,0); 
    xTaskCreatePinnedToCore(wordbook_task    , "wordbook_task",       10000,  NULL, 2, &wordbook_Handle    ,0); 
}
void Device_Screen(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(150);
    for (;;) {
        lv_task_handler();
        refresh_screen();
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        vTaskDelay(_xTicksToWait);
    }
}

void weather_task(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(10000);
    Weather weather;
    for (;;) {
        weather.weather_main();
        xTaskNotifyGive(Screen_Handle); 
        vTaskDelay(_xTicksToWait);
    } 
}
void wordbook_task(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(10000);
    WordBook wordbook;
    for (;;) {
        lv_scr_load(ui_HomePage);
        lv_textarea_set_text(ui_WordbookArea, wordbook.getSingleWord(0));
        lv_obj_invalidate(ui_WordbookArea);
        xTaskNotifyGive(Screen_Handle);
        vTaskDelay(_xTicksToWait);
    }
}