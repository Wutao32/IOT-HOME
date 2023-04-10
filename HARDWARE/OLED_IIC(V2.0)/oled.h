//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : Evk123
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 0.69��OLED �ӿ���ʾ����(STM32F103ZEϵ��IIC)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PB13��SCL��
//              SDA   ��PB15��SDA�� 
//              RES   ��PB11 �������4��iic�ӿ�����ſ��Բ���
//              ----------------------------------------------------------------
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC�˿ڶ���----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���

void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack(void);
//--------------------------------------API--------------------------------------
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Clear_Row(u8 num);
void OLED_Set_Pos(unsigned char x, unsigned char y);

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 flag);
void OLED_ShowCHineseWord(u8 x,u8 y,u8* str,u8 flag);
void OLED_ShowText(u8 x,u8 y,u8* str,u8 flag);
void OLED_ShowNumber(u8 x,u8 y,u16 num,u8 len,u8 size2);
void Display_Menu(void);
#endif  
