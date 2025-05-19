#ifndef CONF_H
#define CONF_H
#include <map>
#include "header.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ui/ui.h"
#include "lvgl.h"
#include "FT6336.h"
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "System/touch.h"
#include "System/display.h"

#include "Application/WordBook.h"
#include "Application/Weather.h"
void SystemInit();
void System_Init(void);
void Applications_Init(void);


void lvgl_task(void* arg);
void idle_task(void* arg);
void screen_task(void* arg);

static bool sleep_state = false;
#endif