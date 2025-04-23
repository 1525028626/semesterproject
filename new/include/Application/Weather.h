#pragma once
#include "System/network.h"  // 包含基类头文件
#include <ArduinoJson.h>
#include "ui/ui.h"

class Weather : public Network {
private:
    static const String XINZHI_KEY;

public:
    using Network::Network;  // 继承基类构造函数

    String xinzhi_url(String ip);
    bool getCityinfo(String& country, String& province, String& city, String& weather, String& temp);
    static void stringSplit(String* dst, String message, char flag);

    void weather_main();
};