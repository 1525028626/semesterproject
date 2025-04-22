#include "Application/WordBook.h"
#include <random>

WordBook::WordBook() {
    if (!initFs()) {
        return;
    }
    if (!SPIFFS.exists(WORDBOOK_FILE)) {
        if (!createWordbook()) {
            return;
        }
    }
}

// 修改头文件引用


bool WordBook::initFs() {  
    if (!SPIFFS.begin(true)) {  
        Serial.println("Failed to mount LittleFS");
        return false;
    }
    return true;
}

bool WordBook::createWordbook() {
    File file = SPIFFS.open(WORDBOOK_FILE, FILE_WRITE);  
    if (!file) {
        Serial.println("Failed to open file for writing");
        return false;
    }
    DynamicJsonDocument doc(6000);
    JsonArray words = doc.createNestedArray("words");
    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write to file");
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool WordBook::addWord(const char* word, const char* translation) {
    File file = SPIFFS.open(WORDBOOK_FILE, FILE_READ);  
    if (!file) {
        Serial.println("Failed to open file for reading");
        return false;
    }
    DynamicJsonDocument doc(6000);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error) {
        Serial.print("Failed to read file: ");
        Serial.println(error.c_str());
        return false;
    }
    JsonArray words = doc["words"];
    JsonObject newWord = words.createNestedObject();
    newWord["word"] = word;
    newWord["translation"] = translation;
    file = SPIFFS.open(WORDBOOK_FILE, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return false;
    }
    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write to file");
        file.close();
        return false;
    }
    file.close();
    return true;
}

void WordBook::printAllWords() {
    File file = SPIFFS.open(WORDBOOK_FILE, FILE_READ);  
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }
    DynamicJsonDocument doc(6000);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error) {
        Serial.print("Failed to read file: ");
        Serial.println(error.c_str());
        return;
    }
    JsonArray words = doc["words"];
    for (JsonObject word : words) {
        const char* wordText = word["word"];
        const char* translation = word["translation"];
        Serial.print("Word: ");
        Serial.print(wordText);
        Serial.print(", Translation: ");
        Serial.println(translation);
    }
}

const char* WordBook::getSingleWord(int choice) {
    static char result[256];
    File file = SPIFFS.open(WORDBOOK_FILE, FILE_READ);  // 改回SPIFFS
    if (!file) {
        strcpy(result, "Failed to open file for reading");
        return result;
    }
    DynamicJsonDocument doc(6000);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if (error) {
        strcpy(result, "Failed to read file: ");
        strcat(result, error.c_str());
        return result;
    }
    JsonArray words = doc["words"];
    int wordCount = words.size();

    if (wordCount == 0) {
        strcpy(result, "No words in the wordbook.");
        return result;
    }

    if (choice == 0) {
        
        srand(static_cast<unsigned int>(time(nullptr)));
    
        int randomIndex = rand() % 32+1;
        JsonObject word = words[randomIndex];
        Serial.print("Random Word: ");
        Serial.print(randomIndex);
        const char* wordText = word["word"];
        const char* translation = word["translation"];
        strcpy(result, wordText);
        strcat(result, "\n");
        strcat(result, translation);
        return result;
    } else if (choice >= 1 && choice <= wordCount) {
        
        JsonObject word = words[choice - 1];
        const char* wordText = word["word"];
        const char* translation = word["translation"];
        strcpy(result, wordText);
        strcat(result, "\n");
        strcat(result, translation);
        return result;
    } else {
        strcpy(result, "Invalid choice.");
        return result;
    }
}