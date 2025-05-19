#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_
#include <Arduino.h>

#define CLK_Pin  10 //CLK
#define MOSI_Pin 9//Din
#define CS_Pin   11
#define RES_Pin  5
#define DC_Pin   4
#define BUSY_Pin 6


#define EPD_W21_MOSI_0	digitalWrite(MOSI_Pin,LOW)
#define EPD_W21_MOSI_1	digitalWrite(MOSI_Pin,HIGH)

#define EPD_W21_CLK_0	digitalWrite(CLK_Pin,LOW)
#define EPD_W21_CLK_1	digitalWrite(CLK_Pin,HIGH)

#define EPD_W21_CS_0 digitalWrite(CS_Pin,LOW)
#define EPD_W21_CS_1 digitalWrite(CS_Pin,HIGH)
#define EPD_W21_DC_0  digitalWrite(DC_Pin,LOW)
#define EPD_W21_DC_1  digitalWrite(DC_Pin,HIGH)
#define EPD_W21_RST_0 digitalWrite(RES_Pin,LOW)
#define EPD_W21_RST_1 digitalWrite(RES_Pin,HIGH)

#define isEPD_W21_BUSY digitalRead(BUSY_Pin)




void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char data);
void EPD_W21_WriteCMD(unsigned char command);
void EPD_GPIO_Init(void);

#endif  //#ifndef _MCU_SPI_H_

/***********************************************************
						end file
***********************************************************/
