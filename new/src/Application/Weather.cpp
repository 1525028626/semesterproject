#include "Application/Weather.h"

const String Weather::XINZHI_KEY = "Szpui2ZH-Eb51B_5T";

void homepage_weather(void* arg){
    Weather weather;
    TickType_t _xTicksToWait = pdMS_TO_TICKS(1000000);
    while(1){
        printf("homepage_weather\n");
        weather.weather_main();// 每10秒更新一次
        vTaskDelay(_xTicksToWait);
    }
}
void weatherpage_weather(void* arg){
    Weather weather;
    TickType_t _xTicksToWait = pdMS_TO_TICKS(1000000);
    while(1){
        weather.weather_daily();// 每10秒更新一次
        vTaskDelay(_xTicksToWait);
    }
}
uint8_t flag_loc=0;
void Weather::weather_main(){
    String country, province, city, weather, temp;
    String loc;
    getCityinfo(country, province, city, weather, temp);
    if(flag_loc==0&&country!=NULL){
        loc = country + "/" + province + "/" + city+" "+temp + "℃";
        flag_loc=1;
    }
    printf("%s\n",loc.c_str());
    lv_textarea_set_text(ui_SecondaryArea,loc.c_str());
}
void Weather::weather_daily(){
    String json = FileHandler::httpGetRequest(xinzhi_url("daily"));
    if(json == "") return; 
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    JsonArray daily = doc["results"][0]["daily"];
    int16_t lows[daily.size()]; // 创建整数数组
    uint8_t index = 0;
    for(JsonObject day : daily) {
        lows[index++] = atoi(day["low"]); // 字符串转整型存入数组
    }
    lv_chart_series_t * ui_Chart1_series_1 = lv_chart_add_series(ui_WeatherChart, lv_color_hex(0x000000),
                                                                 LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_ext_y_array(ui_WeatherChart, ui_Chart1_series_1, lows); // 使用整数数组

    lv_obj_invalidate(ui_WeatherChart);
}

String Weather::xinzhi_url(String type) {
    String url_xinzhi = "http://api.seniverse.com/v3/weather/";
    //https://api.seniverse.com/v3/weather/now.json?key=Szpui2ZH-Eb51B_5T&location=ip&language=zh-Hans&unit=c
    //https://api.seniverse.com/v3/weather/daily.json?key=Szpui2ZH-Eb51B_5T&location=ip&language=zh-Hans&unit=c&start=0&days=7
    if(type == "daily") {
        url_xinzhi += "daily.json?key=";
        url_xinzhi += XINZHI_KEY;
        url_xinzhi += "&location=ip";
        url_xinzhi += "&language=zh-Hans&unit=c&start=0&days=7";
    } 
    else {
        url_xinzhi += "now.json?key=";
        url_xinzhi += XINZHI_KEY;
        url_xinzhi += "&location=ip";
        url_xinzhi += "&language=zh-Hans&unit=c";
    }
    return url_xinzhi;
}

bool Weather::getCityinfo(String& country, String& province, String& city, String& weather, String& temp) {
    String json = FileHandler::httpGetRequest(xinzhi_url("now"));
    DynamicJsonDocument doc(512);
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
