#include "System/FileHandler.h"

String FileHandler::httpGetRequest(const String& url) {
    HTTPClient http;
    http.begin(url);
    return http.GET() == HTTP_CODE_OK ? http.getString() : "";
}

bool FileHandler::parseJson(const String& json, const char* keyPath, String& output) {
    DynamicJsonDocument doc(4096); // 增大缓冲区
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        Serial.printf("JSON解析错误：%s\n", error.c_str());
        return false;
    }
    
    JsonVariant value = doc;
    const char* path = keyPath;
    while (*path) {
        const char* delimiter = strchr(path, '|');
        size_t keyLength = delimiter ? delimiter - path : strlen(path);
        
        String key(path, keyLength);
        if (!value.containsKey(key)) {
            Serial.printf("键路径错误：%s\n", key.c_str());
            return false;
        }
        value = value[key];
        path += keyLength + (delimiter ? 1 : 0);
    }
    
    output = value.isNull() ? "" : value.as<String>();
    return true;
}

void FileHandler::splitString(String* dst, String src, char delimiter) {
    uint8_t index = 0;
    int pos = 0;
    while ((pos = src.indexOf(delimiter)) != -1) {
        dst[index++] = src.substring(0, pos);
        src = src.substring(pos + 1);
    }
    dst[index] = src;
}