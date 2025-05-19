#pragma once
#include "System/network.h"  // 包含基类头文件
#include <ArduinoJson.h>
#include "ui/ui.h"
#include "System/FileHandler.h"

void homepage_weather(void* arg);
void weatherpage_weather(void* arg);
class Weather {
private:
    static const String XINZHI_KEY;

public:  

    String xinzhi_url(String ip);
    bool getCityinfo(String& country, String& province, String& city, String& weather, String& temp);
    static void stringSplit(String* dst, String message, char flag);

    void weather_main();
    void weather_daily();
};