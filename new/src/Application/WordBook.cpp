#include "Application/WordBook.h"
#include <SPIFFS.h>

WordBook::WordBook(const char* path) : count(0), jsonPath(path) {
    load();
}

void WordBook::load() {
    File file = SPIFFS.open(jsonPath);
    if (!file) return;

    StaticJsonDocument<4096> doc;
    deserializeJson(doc, file);
    
    JsonArray arr = doc["words"];
    for (JsonObject obj : arr) {
        if (count >= MAX_WORDS) break;
        strlcpy(words[count], obj["word"], WORD_LEN);
        strlcpy(translations[count], obj["translation"], TRANS_LEN);
        count++;
    }
    file.close();
}

void WordBook::save() {
    StaticJsonDocument<4096> doc;
    JsonArray arr = doc.createNestedArray("words");
    
    for (int i = 0; i < count; i++) {
        JsonObject obj = arr.createNestedObject();
        obj["word"] = words[i];
        obj["translation"] = translations[i];
    }
    
    File file = SPIFFS.open(jsonPath, FILE_WRITE);
    if (file) {
        serializeJson(doc, file);
        file.close();
    }
}

bool WordBook::add(const char* word, const char* trans) {
    if (count >= MAX_WORDS) return false;
    
    strlcpy(words[count], word, WORD_LEN);
    strlcpy(translations[count], trans, TRANS_LEN);
    count++;
    save();
    return true;
}

const char* WordBook::find(const char* word) {
    static char result[TRANS_LEN] = {0};
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            strlcpy(result, translations[i], TRANS_LEN);
            return result;
        }
    }
    return "";
}

const char* WordBook::get(int index) {
    static char output[WORD_LEN + TRANS_LEN + 2] = {0};
    if (index == 0) index = random(1, count + 1);
    
    if (index > 0 && index <= count) {
        snprintf(output, sizeof(output), "%s\n%s", 
                words[index-1], translations[index-1]);
        return output;
    }
    return "";
}