#ifndef WORD_BOOK_H
#define WORD_BOOK_H

#include <SPIFFS.h>
#include <ArduinoJson.h>

class WordBook {
private:
    const char* WORDBOOK_FILE = "/words.json";
    bool initFs();
    bool createWordbook();

public:
    WordBook();
    bool addWord(const char* word, const char* translation);
    void printAllWords();
    const char* getSingleWord(int choice);
};

#endif    