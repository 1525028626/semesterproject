#include "Display_EPD_W21_spi.h"

#include <Arduino.h>
void EPD_GPIO_Init(void)
{	
	  pinMode(BUSY_Pin,  INPUT);
    pinMode(RES_Pin , OUTPUT);
    pinMode(DC_Pin  , OUTPUT);
    
    pinMode(CLK_Pin, OUTPUT);
    pinMode(MOSI_Pin, OUTPUT);
    pinMode(CS_Pin , OUTPUT);

    digitalWrite(CS_Pin , HIGH);
    digitalWrite(CLK_Pin, LOW);
}



void SPI_Write(unsigned char value)                                    
{                                                           
    unsigned char i;

    for(i=0; i<8; i++)   
    {
        EPD_W21_CLK_0;
        if(value & 0x80)
        	EPD_W21_MOSI_1;
        else
        	EPD_W21_MOSI_0;		
        value = (value << 1); 
        EPD_W21_CLK_1; 
    }
}

void EPD_W21_WriteCMD(unsigned char command)
{
  EPD_W21_CS_0;                   
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_CS_1;
}
void EPD_W21_WriteDATA(unsigned char data)
{
  EPD_W21_CS_0;                   
	EPD_W21_DC_1;		// data write
	SPI_Write(data);
	EPD_W21_CS_1;
}



/***********************************************************
						end file
***********************************************************/
