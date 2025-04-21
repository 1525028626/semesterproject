#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define XINZHI_KEY "Szpui2ZH-Eb51B_5T"

class Network
{
// private:
// 	String composeZXaddress(String city);
public:
	void network_init(String ssid, String password);    
    void configWorldTime();
    String xinzhi_url(String city);
    String getWeather(String city);
    bool getCityinfo(String& country, String& province,String& city, String& weather, String& temp);
    void stop(void);
    static void stringSplit(String* dst, String src, char flag);
};


#endif