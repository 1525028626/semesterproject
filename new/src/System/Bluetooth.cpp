#include "System/Bluetooth.h"
#include <string.h>

// 定义全局变量
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristicTx = NULL;
bool deviceConnected = false;
char buffer[20];
void ble_switch_handler(lv_event_t * e) {
    lv_obj_t * sw = lv_event_get_target(e);
    if(lv_obj_has_state(sw, LV_STATE_CHECKED)) {
        Bluetooth ble;
        ble.init("ESP32");
        printf("BLE started\n");
    } else {
        Bluetooth ble;
        ble.stop();
        printf("BLE stopped\n");
    }
}

void Bluetooth::init(const char* deviceName){ 
    BLEDevice::init(deviceName);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService* pService = pServer->createService(SERVICE_UUID);
    pCharacteristicTx = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pCharacteristicTx->addDescriptor(new BLE2902());
    BLECharacteristic *pCharacteristicRx = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_RX,
                       BLECharacteristic::PROPERTY_WRITE
                     );
    pCharacteristicRx->setCallbacks(new MyCallbacks());  // 设置回调
    pCharacteristicTx->addDescriptor(new BLE2902());


    pService->start();


    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();
}

void Bluetooth::sendData(std::string data){
    pCharacteristicTx->setValue(data);
    pCharacteristicTx->notify();
}

void Bluetooth::receiveData(char* receiveData){
    // 将 buffer 中的数据复制给 receiveData
    strcpy(receiveData, buffer);
    // 清空 buffer
    memset(buffer, 0, sizeof(buffer));
}

bool Bluetooth::isConnected() {
    return deviceConnected;  // 直接返回全局连接状态变量
}

void Bluetooth::stop() {
    // 停止广播
    BLEDevice::getAdvertising()->stop();
    
    // 断开所有连接
    if (pServer) {
        pServer->getConnectedCount();
        // 断开所有客户端连接
        pServer->disconnect(pServer->getConnId());
    }
    
    // 停止BLE服务
    BLEDevice::deinit();
    
    // 重置状态
    deviceConnected = false;
    pServer = NULL;
    pCharacteristicTx = NULL;
}

