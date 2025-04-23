#include "Application/Weather.h"
const String Weather::XINZHI_KEY = "Szpui2ZH-Eb51B_5T";
void Weather::weather_main(){
    String country, province, city, weather, temp;
    String loc;
    if(getCityinfo(country, province, city, weather, temp)){
        loc = country + "/" + province + "/" + city+" "+temp + "℃";
            lv_textarea_set_text(ui_TimeArea,loc.c_str());
            lv_obj_invalidate(ui_TimeArea);
    }  
}
String Weather::xinzhi_url(String ip) {
    String url_xinzhi = "https://api.seniverse.com/v3/weather/now.json?key=";
    url_xinzhi += XINZHI_KEY;
    url_xinzhi += "&location=";
    url_xinzhi += ip;
    url_xinzhi += "&language=zh-Hans&unit=c";
    return url_xinzhi;
}
bool Weather::getCityinfo(String& country, String& province, String& city, String& weather, String& temp) {
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


void Weather::stringSplit(String* dst, String message, char flag){
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
