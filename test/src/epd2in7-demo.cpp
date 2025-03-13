/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include  "head.h"

// 定义任务句柄
TaskHandle_t epdTaskHandle = NULL;

/* Entry point ----------------------------------------------------------------*/
void epdTask(void *pvParameters) {
  printf("EPD_2IN7_test Demo\r\n");
  DEV_Module_Init();

  printf("e-Paper Init and Clear...\r\n");
  EPD_2IN7_Init();
  Serial.print("11");
  EPD_2IN7_Clear();
  DEV_Delay_ms(500);

  //Create a new image cache
  UBYTE *BlackImage;
  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
  UWORD Imagesize = ((EPD_2IN7_WIDTH % 8 == 0) ? (EPD_2IN7_WIDTH / 8 ) : (EPD_2IN7_WIDTH / 8 + 1)) * EPD_2IN7_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    while (1);
  }
  printf("Paint_NewImage\r\n");
  Paint_NewImage(BlackImage, EPD_2IN7_WIDTH, EPD_2IN7_HEIGHT, 270, WHITE);

  printf("show image for array\r\n");
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  Paint_DrawBitMap(gImage_2in7);
  EPD_2IN7_Display(BlackImage);
  DEV_Delay_ms(2000);


  printf("Goto Sleep...\r\n");
  EPD_2IN7_Init();
  EPD_2IN7_Clear();
  EPD_2IN7_Sleep();
  free(BlackImage);
  BlackImage = NULL;

}
void setup() {
  // 创建任务
  xTaskCreate(
    epdTask,        // 任务函数
    "epdTask",      // 任务名称
    4096,           // 任务堆栈大小
    NULL,           // 任务参数
    1,              // 任务优先级
    &epdTaskHandle  // 任务句柄
  ); 
}

/* The main loop -------------------------------------------------------------*/
void loop() {
  // FreeRTOS 会自动调度任务，主循环可以留空
 
}

