#pragma once
#include <ArduinoJson.h>

#define MAX_WORDS 100
#define WORD_LEN 32
#define TRANS_LEN 64

class WordBook {
private:
    char words[MAX_WORDS][WORD_LEN];
    char translations[MAX_WORDS][TRANS_LEN];
    int count;
    const char* jsonPath;
    
    void load();
    void save();

public:
    WordBook(const char* path = "/words.json");
    
    bool add(const char* word, const char* trans);
    bool remove(const char* word);
    const char* find(const char* word);
    const char* get(int index);
    int total();
};