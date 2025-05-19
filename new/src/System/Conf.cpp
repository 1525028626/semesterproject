#include "System/Conf.h"
#include "System/PageManager.h"

TaskHandle_t idle_Handle;
TaskHandle_t lvgl_Handle;
TaskHandle_t screen_Handle;



extern volatile bool idle_wake_flag;
void  SystemInit() {
    Serial.begin(115200);
    EPD_GPIO_Init();
    lvgl_set_init();
    touch_init();
    ui_init();
    System_Init();
    Applications_Init();

    lv_scr_load(ui_HomePage);
    lv_obj_add_event_cb(ui_wifiSwitch, wifi_switch_handler, LV_EVENT_VALUE_CHANGED, NULL);
}
void System_Init(void) {
    xTaskCreatePinnedToCore(lvgl_task,      "lvgl_task",       10000,  NULL, 0, &lvgl_Handle,    0);
    xTaskCreatePinnedToCore(screen_task,      "screen_task",       10000,  NULL, 1, &screen_Handle,    0);
    xTaskCreatePinnedToCore(idle_task,      "idle_task",       10000,  NULL, 2, &idle_Handle,    0);
}

void Applications_Init(void) {
    // 注册天气页面
    PageManager::register_task_page(ui_HomePage, homepage_weather, 
                                  "WeatherTask", 10000, 1,1);
    
    // 注册单词本页面
    PageManager::register_task_page(ui_HomePage, homepage_wordbook, 
                                  "WordbookTask", 10000, 2,1);

    // 注册天气页面，柱状图
    PageManager::register_task_page(ui_WeatherPage,weatherpage_weather,
                                  "weatherpage_weather", 10000, 2,1);
}
void lvgl_task(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(20);
    for (;;) {      
        lv_task_handler();
        vTaskDelay(_xTicksToWait);
    } 
}
void screen_task(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(100);
    for (;;) {
        if(lvgl_is_display_updated()) {
            refresh_screen();
        }
        vTaskDelay(_xTicksToWait);
    }
}
void idle_task(void* arg) {
    const TickType_t check_interval = pdMS_TO_TICKS(1000); // 改为每秒检测
    const uint32_t inactive_threshold = 60000; // 60秒阈值

    for (;;) {
        uint32_t inactive_time = lv_disp_get_inactive_time(NULL);
        if(inactive_time >= inactive_threshold&&sleep_state == false) {
            sleep_state = true;
            idle_wake_flag=false;
            lv_scr_load(ui_HomePage);      
        }
        vTaskDelay(check_interval);
    }
}


