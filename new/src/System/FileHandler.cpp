#include "System/FileHandler.h"

String FileHandler::httpGetRequest(const String& url) {
    HTTPClient http;
    http.begin(url);
    return http.GET() == HTTP_CODE_OK ? http.getString() : "";
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