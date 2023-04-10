#ifndef __IIC_OLED_H__
#define __IIC_OLED_H__
#include "STM32f10x.h"
#include "u8g2.h"
void OLED_Init(u8g2_t u8g2);
//void OLED12864_IoInit(void);//IO初始化
void IIC_OLED_SDA_HIGH(void);	
void IIC_OLED_SDA_LOW(void);
void IIC_OLED_SCL_HIGH(void);	
void IIC_OLED_SCL_LOW(void);
#endif
