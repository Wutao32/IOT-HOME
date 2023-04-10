//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// �о�԰����
// ���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//   �� �� ��   : main.c
//   �� �� ��   : v2.0
//   ��    ��   : Evk123
//   ��������   : 2014-0101
//   ����޸�   :
//   ��������   : 0.69��OLED �ӿ���ʾ����(STM32F103ZEϵ��IIC)
//               ˵��:
//               ----------------------------------------------------------------
//               GND   ��Դ��
//               VCC   ��5V��3.3v��Դ
//               SCL   ��PB6��SCL��
//               SDA   ��PB7��SDA��
//               RES   ��PB11 �������4��iic�ӿ�����ſ��Բ���
//               ----------------------------------------------------------------
// Copyright(C) �о�԰����2014/3/16
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"
#include "string.h"
#define IIC_OLED_SDA_Pin GPIO_Pin_1
#define IIC_OLED_SDA_GPIO GPIOA
#define IIC_OLED_SDA_GPIO_CLK RCC_APB2Periph_GPIOA

#define IIC_OLED_SCL_Pin GPIO_Pin_0
#define IIC_OLED_SCL_GPIO GPIOA
#define IIC_OLED_SCL_GPIO_CLK RCC_APB2Periph_GPIOA
// OLED���Դ�
// ��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{

	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	OLED_SCLK_Set();
	//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

void IIC_Wait_Ack()
{
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m, da;
	da = IIC_Byte;
	OLED_SCLK_Clr();
	for (i = 0; i < 8; i++)
	{
		m = da;
		m = m & 0x80;
		if (m == 0x80)
		{
			OLED_SDIN_Set();
		}
		else
			OLED_SDIN_Clr();
		da = da << 1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // Slave address,SA0=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00); // write command
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // D/C#=0; R/W#=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40); // write data
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
	if (cmd)
	{
		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}

/********************************************************************************
* @��������         OLED_Set_Pos
* @����������       �ƶ���굽ָ��λ��
* @�������ߣ�       ì�ܾۺ���
* @���������
					������    ��������  ��������
					@x��      unsignedchar �����꣬0~128
					@y��      unsignedchar �����꣬0~7
* @����ֵ��         void
********************************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}
// ����OLED��ʾ
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC����
	OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}
// �ر�OLED��ʾ
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC����
	OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}

/********************************************************************************
 * @��������         OLED_Clear
 * @����������       ��0x00���GCRAM��ʹ����Ļȫ��
 * @�������ߣ�       ì�ܾۺ���
 * @����ֵ��         void
 ********************************************************************************/
void OLED_Clear(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // ����ҳ��ַ��0~7��
		OLED_WR_Byte(0x00, OLED_CMD);	  // ������ʾλ�á��е͵�ַ
		OLED_WR_Byte(0x10, OLED_CMD);	  // ������ʾλ�á��иߵ�ַ
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(0, OLED_DATA);
	} // ������ʾ
}

/********************************************************************************
* @��������         OLED_Clear_Row
* @����������       ���ָ����
* @�������ߣ�       ì�ܾۺ���
* @���������
					������    ��������  ��������
					@y��      u8         ������0~7
* @����ֵ��         void
********************************************************************************/
void OLED_Clear_Row(u8 y)
{
	u8 n;
	OLED_Set_Pos(0, y);
	for (n = 0; n < 128; n++)
		OLED_WR_Byte(0, OLED_DATA);
}
void OLED_On(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // ����ҳ��ַ��0~7��
		OLED_WR_Byte(0x00, OLED_CMD);	  // ������ʾλ�á��е͵�ַ
		OLED_WR_Byte(0x10, OLED_CMD);	  // ������ʾλ�á��иߵ�ַ
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(1, OLED_DATA);
	} // ������ʾ
}

/********************************************************************************
* @��������         OLED_ShowChar
* @����������       ��ָ��λ����ʾռ��8*16�ĵ���ASCII�ַ�
* @�������ߣ�       ì�ܾۺ���
* @���������
					������    ��������  ��������
					@x��      u8         �����꣬0~127
					@y��      u8         �����꣬0~7
					@chr��    u8         Ҫ��ʾ��ASCII�ַ�
					@flag��   u8         ���ױ�־����0ʱ������ʾ
* @����ֵ��         void
* @������
********************************************************************************/
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 flag)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; // �õ�ƫ�ƺ��ֵ
	if (x > Max_Column - 1)
	{
		x = 0;
		y = y + 2;
	}
	OLED_Set_Pos(x, y);
	for (i = 0; i < 8; i++)
	{
		if (flag == 1)
			OLED_WR_Byte(~F8X16[c * 16 + i], OLED_DATA);
		else
			OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
	}

	OLED_Set_Pos(x, y + 1);
	for (i = 0; i < 8; i++)
	{
		if (flag == 1)
			OLED_WR_Byte(~F8X16[c * 16 + i + 8], OLED_DATA);
		else
			OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
	}
}

/********************************************************************************
* @��������         OLED_ShowCHineseWord
* @����������       ��ָ��λ����ʾռ��16*16�ĵ��� ����
* @�������ߣ�       ì�ܾۺ���
* @���������
					������    ��������  ��������
					@x��      u8         �����꣬0~127
					@y��      u8         �����꣬0~7
					@str��    u8*        Ҫ��ʾ�ĺ���
					@flag��   u8         ���ױ�־����0ʱ������ʾ
* @����ֵ��         void
* @������
********************************************************************************/
void OLED_ShowCHineseWord(u8 x, u8 y, u8 *str, u8 flag)
{
	u8 t = 0;
	u16 index;
	for (index = 0; index < sizeof(Hzk) / 35; index++)
	{
		if (Hzk[index].name[0] == str[0] && Hzk[index].name[1] == str[1]) // �ԱȺ�������λ��
		{
			OLED_Set_Pos(x, y);		 // ����OLED���λ��
			for (t = 0; t < 16; t++) // ��д�����ϰ벿������
			{
				if (flag == 0)
					OLED_WR_Byte(Hzk[index].dat[t], OLED_DATA);
				else
					OLED_WR_Byte(~Hzk[index].dat[t], OLED_DATA);
			}
			OLED_Set_Pos(x, y + 1);	 // ����OLED���λ��
			for (t = 0; t < 16; t++) // ��д�����°벿������
			{
				if (flag == 0)
					OLED_WR_Byte(Hzk[index].dat[t + 16], OLED_DATA);
				else
					OLED_WR_Byte(~Hzk[index].dat[t + 16], OLED_DATA);
			}
		}
	}
}

/********************************************************************************
* @��������         OLED_ShowText
* @����������       ��ָ��λ����ʾ��Ӣ���ַ���
* @�������ߣ�       ì�ܾۺ���
* @���������
					������    ��������  ��������
					@x��      u8         �����꣬0~127
					@y��      u8         �����꣬0~7
					@str��    u8*        Ҫ��ʾ���ַ���
					@flag��   u8         ���ױ�־����0ʱ������ʾ
* @����ֵ��         void
* @������
********************************************************************************/
void OLED_ShowText(u8 x, u8 y, u8 *str, u8 flag)
{
	u8 tempstr[3] = {'\0'};
	while (*str != '\0')
	{
		if (*str & 0x80)
		{
			tempstr[0] = *str++;
			tempstr[1] = *str++;
			OLED_ShowCHineseWord(x, y, tempstr, flag);
			x += 16;
			if (*str & 0x80) // �ж���һ���ַ������Ļ���Ӣ��
			{
				if (x >= 112)
				{
					y++;
					y++;
					x = 0;
				} // �޸ĵ�ַ
			}
			else
			{
				if (x >= 120)
				{
					y++;
					y++;
					x = 0;
				} // �޸ĵ�ַ
			}
		}
		else
		{
			OLED_ShowChar(x, y, *str++, flag);
			x += 8;
			if (*str & 0x80)
			{
				if (x >= 112)
				{
					y++;
					y++;
					x = 0;
				} // �޸ĵ�ַ
			}
			else
			{
				if (x >= 120)
				{
					y++;
					y++;
					x = 0;
				} // �޸ĵ�ַ
			}
		}
	}
}
// ��ʼ��SSD1306
void OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};			   // ����GPIO�ṹ��
	RCC_APB2PeriphClockCmd(IIC_OLED_SDA_GPIO_CLK, ENABLE); // ����GPIOģ���ʱ��
	RCC_APB2PeriphClockCmd(IIC_OLED_SCL_GPIO_CLK, ENABLE); // ����GPIOģ���ʱ��

	GPIO_InitStructure.GPIO_Pin = IIC_OLED_SDA_Pin;	   // ����SDA�˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // ����GPIO��ģʽΪ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ���ö˿��������Ϊ���������
	GPIO_Init(IIC_OLED_SDA_GPIO, &GPIO_InitStructure); // ��ʼ��GPIOΪ�����������ģʽ

	GPIO_InitStructure.GPIO_Pin = IIC_OLED_SCL_Pin;	   // ����SCL�˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // ����GPIO��ģʽΪ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ���� I/O �ڵ��ٶ�Ϊ��50MHz������ѡ��GPIO_Speed_2MHz��GPIO_Speed_10MHz
	GPIO_Init(IIC_OLED_SCL_GPIO, &GPIO_InitStructure); // ��ʼ��GPIOΪ�����������ģʽ

	DelayMs(200);

	OLED_WR_Byte(0xAE, OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0x00, OLED_CMD); //���õ��е�ַ
	OLED_WR_Byte(0x10, OLED_CMD); //���ø��е�ַ
	OLED_WR_Byte(0x40, OLED_CMD); //������ʼ�е�ַ,��ӳ��RAM��ʾ��ʼ��(0x00~0x3F)
	OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
	OLED_WR_Byte(0x81, OLED_CMD); // contract control
	OLED_WR_Byte(0xFF, OLED_CMD); //--128
	OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap
	OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
	OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
	OLED_WR_Byte(0x00, OLED_CMD); //

	OLED_WR_Byte(0xD5, OLED_CMD); // set osc division
	OLED_WR_Byte(0x80, OLED_CMD); //

	OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
	OLED_WR_Byte(0x05, OLED_CMD); //

	OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
	OLED_WR_Byte(0xF1, OLED_CMD); //

	OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion
	OLED_WR_Byte(0x12, OLED_CMD); //

	OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh
	OLED_WR_Byte(0x30, OLED_CMD); //

	OLED_WR_Byte(0x8D, OLED_CMD);//���ó�������/����
	OLED_WR_Byte(0x14, OLED_CMD);//����(0x10����,0x14����)

	OLED_WR_Byte(0xAF, OLED_CMD); ////������ʾ
	OLED_Clear();
}
void Display_Menu()
{
	OLED_ShowText(0,0,(u8*)"��״̬: δ��",0);
	OLED_ShowText(0,2,(u8*)"�¶�: --  ��",0);
	OLED_ShowText(0,4,(u8*)"ʪ��: --  %RH",0);
	OLED_ShowText(0,6,(u8*)"������״̬�쳣",0);
}

uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
void OLED_ShowNumber(u8 x,u8 y,u16 num,u8 len,u8 size2)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
}
