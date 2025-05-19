#include "FT6336.h"
#include "i2c.h"


unsigned char FT6336_read_firmware_id(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_FIRMARE_ID);
}

unsigned char FT6336_read_device_mode(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_DEVICE_MODE);
}

unsigned char FT6336_read_td_status(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TD_STATUS);
}


unsigned char FT6336_read_touch1_event(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_EVENT)>>4;
}

unsigned char FT6336_read_touch1_id(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_ID)>>4;
}


unsigned  FT6336_read_touch1_x(void)
{
	unsigned char read_buf[2];
	i2c_read_addr_str(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_X,2,read_buf);
	return ((read_buf[0]&0x0f)<<8)|read_buf[1];
}

unsigned  FT6336_read_touch1_y(void)
{
	unsigned char read_buf[2];
	i2c_read_addr_str(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH1_Y,2,read_buf);
	return ((read_buf[0]&0x0f)<<8)|read_buf[1];
}

//???????
unsigned char FT6336_read_touch2_event(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH2_EVENT)>>4;
}

unsigned char FT6336_read_touch2_id(void)
{
	return i2c_read_addr_byte(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH2_ID)>>4;
}

unsigned  FT6336_read_touch2_x(void)
{
	unsigned char read_buf[2];
	i2c_read_addr_str(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH2_X,2,read_buf);
	return ((read_buf[0]&0x0f)<<8)|read_buf[1];
}

unsigned  FT6336_read_touch2_y(void)
{
	unsigned char read_buf[2];
	i2c_read_addr_str(I2C_ADDR_FT6336,FT6336_ADDR_TOUCH2_Y,2,read_buf);
	return ((read_buf[0]&0x0f)<<8)|read_buf[1];
}


const unsigned  FT6236_TPX_TBL[5]=
{
	FT_TP1_REG,
	FT_TP2_REG,
	FT_TP3_REG,
	FT_TP4_REG,
	FT_TP5_REG
};

//???????FPC???��?
////y??////////    //264x176
							//
							//
							//x??
							//
							//
unsigned char touch_count=0;
TouchPointRefTypeDef TPR_Structure; 
void FT6336_Scan(void)
{
	unsigned char i=0;
	unsigned char sta = 0;
	unsigned char buf[4] = {0};  
	unsigned char gestid=0;
	FT6336_RD_Reg(0x02,&sta,1);//????????????  
  touch_count=sta;	
	FT6336_RD_Reg(0x01,&gestid,1);//????????????
	
 	if(sta & 0x0f)	//?��?????��??????��?0x02????????4��?????��???????
 	{
 		TPR_Structure.TouchSta = ~(0xFF << (sta & 0x0F));	//~(0xFF << (sta & 0x0F))??????????????????????��???
 		for(i=0;i<2;i++)	                                //????��??????1-5???????
 		{
 			if(TPR_Structure.TouchSta & (1<<i))			    //?????????????
 			{											    //???????????????????????????
 				FT6336_RD_Reg(FT6236_TPX_TBL[i],buf,4);	//???XY?????
				TPR_Structure.x[i]=((unsigned int)(buf[0]&0X0F)<<8)+buf[1];  
				TPR_Structure.y[i]=((unsigned int)(buf[2]&0X0F)<<8)+buf[3];
 				if((buf[0]&0XC0)!=0X80)
 				{
					TPR_Structure.x[i]=TPR_Structure.y[i]=0;//??????contact????????????��	
					TPR_Structure.TouchSta &=0xe0;	//???????????��???
					return;
				}
 			}
 		}
 		TPR_Structure.TouchSta |= TP_PRES_DOWN;     //???????��??
 	}
 	else
 	{
 		if(TPR_Structure.TouchSta &TP_PRES_DOWN) 	//????????��?
 			TPR_Structure.TouchSta &= ~0x80;        //??????????	
 		else
 		{
 			TPR_Structure.x[0] = 0;
 			TPR_Structure.y[0] = 0;
 			TPR_Structure.TouchSta &=0xe0;	//???????????��???
 		}
 	}
}
extern volatile bool ft6336_touch_flag;

void FT6336_IRQ(uint& last_x,uint& last_y){
	last_x=0;
	last_y=0;
	 if(ft6336_touch_flag)//Touch is OK
    {  
		ft6336_touch_flag = false;
        
        // 清除中断标志
        uint8_t clear_reg[2] = {0x00, 0x00};
        i2c_write_addr_str(I2C_ADDR_FT6336, 0x00, 2, clear_reg);
      TPR_Structure.TouchSta |= TP_COORD_UD;       
      if(TPR_Structure.TouchSta & TP_COORD_UD) {
            FT6336_Scan();
            last_x=TPR_Structure.x[0];
            last_y=TPR_Structure.y[0];
            // 保留原始坐标直到新的有效数据到来
            // 移除坐标清零操作
        }
  }
}
