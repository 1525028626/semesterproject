#pragma once // 添加头文件保护
#include <WiFi.h>
#include <HTTPClient.h>
#include <stdlib.h>
#include "ui/ui.h"
#include "lvgl.h"
#define WIFI_SSID "9875"
#define WIFI_PASS "qwer9876"
void wifi_switch_handler(lv_event_t * e);
class Network {
private:
    String ssid;    // 移除直接初始化
    String password;
    
public:
    explicit Network();
    void network_init(String ssid, String password);
    void stop(void);
    void configWorldTime();
};