#ifndef CONF_H
#define CONF_H
#include "header.h"
#include <Arduino.h>
#include "DEV_Config.h"
#include "EPD.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "System/display.h"
#include "ui/ui.h"

#include "Application/WordBook.h"

void SystemInit();
void Devices_Init(void);
void Applications_Init(void);


void Device_Screen(void* arg);
void network_task(void* arg);
void wordbook_task(void* arg);



#endif