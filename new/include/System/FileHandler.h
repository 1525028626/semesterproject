#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

class FileHandler {
public:
    static String httpGetRequest(const String& url);
    static bool parseJson(const String& json, const char* keyPath, String& output);
    static void splitString(String* dst, String src, char delimiter);
};