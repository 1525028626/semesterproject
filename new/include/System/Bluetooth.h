#pragma once
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "lvgl.h"
#include "ui/ui.h"
#define SERVICE_UUID           "12345678-1234-1234-1234-1234567890AB"
#define CHARACTERISTIC_UUID_RX "12345678-1234-1234-1234-1234567890AC"
#define CHARACTERISTIC_UUID_TX "12345678-1234-1234-1234-1234567890AD"
void ble_switch_handler(lv_event_t * e);
// 声明全局变量
extern BLEServer* pServer;
extern BLECharacteristic* pCharacteristicTx;
extern bool deviceConnected;
extern char buffer[20]; 

// 回调函数：连接、断开时的处理
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        printf("BLE device connected.\n");
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        printf("BLE device disconnected.\n");
    }
};

// 在文件顶部添加回调类定义
class MyCallbacks: public BLECharacteristicCallbacks {
    // 在MyCallbacks类中添加
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue();
        const size_t MAX_LEN = 20; // BLE单次最大传输长度
        if (rxValue.length() > 0) {
            size_t print_len = rxValue.length() > MAX_LEN ? MAX_LEN : rxValue.length();
            strncpy(buffer, rxValue.data(), sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = '\0'; 
            lv_textarea_set_text(ui_MainArea, buffer);
            printf("Received Value: %.*s\n", print_len, rxValue.data());
        }
    }
};

class Bluetooth{
    public:
     void init(const char* deviceName = "ESP32");
     void sendData(std::string data);
     void receiveData(char* receiveData);
     bool isConnected();
     void stop();
     
};



