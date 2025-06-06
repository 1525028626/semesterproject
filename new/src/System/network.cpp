#include "System/network.h"
#include "Application/Weather.h"

void wifi_switch_handler(lv_event_t * e) {
    lv_obj_t * sw = lv_event_get_target(e);
    if(lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        Network network;
        printf("wifi on\n");
    } else {
        printf("wifi off\n");
        Network network;
        network.stop();
        WiFi.mode(WIFI_OFF); // 新增：彻底关闭WiFi射频
        ESP.restart(); // 新增：定期重启解决内存碎片
    }
}
// 添加构造函数实现
Network::Network() {
    ssid = WIFI_SSID;    // 使用宏定义值
    password = WIFI_PASS;
    network_init(ssid, password);
    configWorldTime();
    configWorldTime();
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
       int current_year = timeinfo.tm_year + 1900;
            int current_month = timeinfo.tm_mon + 1; 
            int current_day = timeinfo.tm_mday;
            lv_calendar_set_today_date(ui_Calendar1, current_year ,current_month,current_day);
            lv_calendar_set_showed_date(ui_Calendar1, current_year ,current_month);
    }
   
     
}

void Network::stop(void) { 
    WiFi.disconnect(true); 
}
void Network::network_init(String ssid, String password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        printf(".");
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
            
}

