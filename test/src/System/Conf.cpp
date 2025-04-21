//系统初始化的内容

#include "head.h"

TaskHandle_t Screen_Handle;
TaskHandle_t network_Handle;

Network wifi;
void system_init() {
    //屏幕初始化
    DEV_Module_Init();
    lvgl_set_init();
    ui_init();

    wifi.network_init("9875", "qwer9876");
   
    //设备初始化
    Devices_Init();
    //功能初始化
    Functions_Init();
}
void Devices_Init(void) {
    xTaskCreatePinnedToCore(Device_Screen,      "Dev.Screen",       10000,  NULL, 1, &Screen_Handle,    0);
}
void Functions_Init(void) {
    xTaskCreatePinnedToCore(network_weather ,"network_weather", 10000, NULL, 3, &network_Handle, 1);
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
void network_weather(void* arg) {
    TickType_t _xTicksToWait = pdMS_TO_TICKS(10000);
    String country, province, city, weather, temp;
    String loc;
    for (;;) {
        wifi.configWorldTime();
        if (wifi.getCityinfo(country, province, city, weather, temp)) {
            loc = country + "/" + province + "/" + city;
            temp = temp + "°";
            Serial.println(loc + " " + weather + " " + temp);           
        }     
        xTaskNotifyGive(Screen_Handle);     
        vTaskDelay(_xTicksToWait);
    }
}