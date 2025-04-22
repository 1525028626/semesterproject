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
int i=0;
void network_task(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(35000);
    for (;;) {
        if(i==0){
            i=1;
            lv_scr_load(ui_HomePage);
            lv_obj_invalidate(ui_HomePage);
            logmy("sc1");
        }else{
            i=0;
            lv_scr_load(ui_CalendarPage);
            lv_obj_invalidate(ui_CalendarPage);
            logmy("sc2");
        }
        xTaskNotifyGive(Screen_Handle); 
        vTaskDelay(_xTicksToWait);
    } 
}
void wordbook_task(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(10000);
    WordBook wordbook("/words.json");
    for (;;) {
        lv_textarea_set_text(ui_TextArea1, wordbook.get(0));
        vTaskDelay(_xTicksToWait);
    }
}