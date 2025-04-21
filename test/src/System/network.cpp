#include "head.h"

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

void Network::stop(void) { WiFi.disconnect(true); }

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
    lv_textarea_set_text(ui_TextArea1, timeStringBuff);
    lv_obj_invalidate(ui_TextArea1);
}

String Network::xinzhi_url(String ip) {
    String url_xinzhi = "https://api.seniverse.com/v3/weather/now.json?key=";
    url_xinzhi += XINZHI_KEY;
    url_xinzhi += "&location=";
    url_xinzhi += ip;
    url_xinzhi += "&language=zh-Hans&unit=c";
    return url_xinzhi;
}
bool Network::getCityinfo(String& country, String& province, String& city, String& weather, String& temp) {
    HTTPClient http;
    DynamicJsonDocument doc(1024);
    /* Compose url */
    String url_xinzhi = xinzhi_url("ip");
    http.begin(url_xinzhi);
    int httpGet = http.GET();
    if (httpGet > 0) {
        if (httpGet == HTTP_CODE_OK) {
            String json = http.getString();
            deserializeJson(doc, json);
            String loc = doc["results"][0]["location"]["path"].as<String>();
            String loc_split[4];
            stringSplit(loc_split, loc, ',');
            city = loc_split[1];
            province = loc_split[2];
            country = loc_split[3];
            weather = doc["results"][0]["now"]["text"].as<String>();
            temp = doc["results"][0]["now"]["temperature"].as<String>();
            return true;
        } else {
            Serial.printf("ERROR!");
            return false;
        }
    } else {
        Serial.printf("ERROR!");
        return false;
    }
}


void Network::stringSplit(String* dst, String message, char flag){
    int flag_position;
    uint8_t num = 0;
    /* Split String */
    do {
        flag_position = message.indexOf(flag);
        if (flag_position != -1) {
            dst[num++] = message.substring(0, flag_position);
            message = message.substring(flag_position + 1, message.length());
        } else {
            dst[num] = message;
        }
    } while (flag_position >= 0);
}