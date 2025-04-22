#include "System/Conf.h"
TaskHandle_t Screen_Handle;
TaskHandle_t network_Handle;
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
    xTaskCreatePinnedToCore(network_task    , "network_task",       10000,  NULL, 2, &network_Handle    ,0); 
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

void network_task(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(35000);
    for (;;) {
        lv_scr_load(ui_CalendarPage);
        lv_obj_invalidate(ui_CalendarPage);
         
        xTaskNotifyGive(Screen_Handle); 
        vTaskDelay(_xTicksToWait);
    } 
}
void wordbook_task(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(10000);
    WordBook wordbook;
    for (;;) {
        lv_scr_load(ui_HomePage);
        lv_textarea_set_text(ui_TextArea1, wordbook.getSingleWord(0));
        lv_obj_invalidate(ui_TextArea1);
        xTaskNotifyGive(Screen_Handle);
        vTaskDelay(_xTicksToWait);
    }
}