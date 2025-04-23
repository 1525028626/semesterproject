#include "System/network.h"
#include "Application/Weather.h"

// 添加构造函数实现
Network::Network() {
    ssid = WIFI_SSID;    // 使用宏定义值
    password = WIFI_PASS;
    network_init(ssid, password);
}

void Network::stop(void) { 
    WiFi.disconnect(true); 
}
void Network::network_init(String ssid, String password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); 
}

void Network::configWorldTime() {
    struct tm timeinfo;
    char timeStringBuff[50];
    if (!getLocalTime(&timeinfo)) {
        configTime(8 * 3600, 0, "ntp1.aliyun.com", "ntp2.aliyun.com", "ntp3.aliyun.com");
        return;
    }
    strftime(timeStringBuff, sizeof(timeStringBuff), "%F %T %A", &timeinfo);
    /* format output: 2021-10-24 23:00:44 Sunday */
    Serial.println(timeStringBuff);
    
}

