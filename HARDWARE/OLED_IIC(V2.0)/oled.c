//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 中景园电子
// 店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//   文 件 名   : main.c
//   版 本 号   : v2.0
//   作    者   : Evk123
//   生成日期   : 2014-0101
//   最近修改   :
//   功能描述   : 0.69寸OLED 接口演示例程(STM32F103ZE系列IIC)
//               说明:
//               ----------------------------------------------------------------
//               GND   电源地
//               VCC   接5V或3.3v电源
//               SCL   接PB6（SCL）
//               SDA   接PB7（SDA）
//               RES   接PB11 如果是用4针iic接口这个脚可以不接
//               ----------------------------------------------------------------
// Copyright(C) 中景园电子2014/3/16
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
// OLED的显存
// 存放格式如下.
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
* @函数名：         OLED_Set_Pos
* @函数描述：       移动光标到指定位置
* @函数作者：       矛盾聚合体
* @输入参数：
					参数名    参数类型  参数描述
					@x：      unsignedchar 列坐标，0~128
					@y：      unsignedchar 行坐标，0~7
* @返回值：         void
********************************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}
// 开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}
// 关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}

/********************************************************************************
 * @函数名：         OLED_Clear
 * @函数描述：       用0x00填充GCRAM，使得屏幕全黑
 * @函数作者：       矛盾聚合体
 * @返回值：         void
 ********************************************************************************/
void OLED_Clear(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(0, OLED_DATA);
	} // 更新显示
}

/********************************************************************************
* @函数名：         OLED_Clear_Row
* @函数描述：       清除指定行
* @函数作者：       矛盾聚合体
* @输入参数：
					参数名    参数类型  参数描述
					@y：      u8         行数，0~7
* @返回值：         void
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
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(1, OLED_DATA);
	} // 更新显示
}

/********************************************************************************
* @函数名：         OLED_ShowChar
* @函数描述：       在指定位置显示占宽8*16的单个ASCII字符
* @函数作者：       矛盾聚合体
* @输入参数：
					参数名    参数类型  参数描述
					@x：      u8         列坐标，0~127
					@y：      u8         行坐标，0~7
					@chr：    u8         要显示的ASCII字符
					@flag：   u8         反白标志，非0时反白显示
* @返回值：         void
* @其他：
********************************************************************************/
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 flag)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; // 得到偏移后的值
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
* @函数名：         OLED_ShowCHineseWord
* @函数描述：       在指定位置显示占宽16*16的单个 汉字
* @函数作者：       矛盾聚合体
* @输入参数：
					参数名    参数类型  参数描述
					@x：      u8         列坐标，0~127
					@y：      u8         行坐标，0~7
					@str：    u8*        要显示的汉字
					@flag：   u8         反白标志，非0时反白显示
* @返回值：         void
* @其他：
********************************************************************************/
void OLED_ShowCHineseWord(u8 x, u8 y, u8 *str, u8 flag)
{
	u8 t = 0;
	u16 index;
	for (index = 0; index < sizeof(Hzk) / 35; index++)
	{
		if (Hzk[index].name[0] == str[0] && Hzk[index].name[1] == str[1]) // 对比汉字区码位码
		{
			OLED_Set_Pos(x, y);		 // 设置OLED光标位置
			for (t = 0; t < 16; t++) // 先写汉字上半部分数据
			{
				if (flag == 0)
					OLED_WR_Byte(Hzk[index].dat[t], OLED_DATA);
				else
					OLED_WR_Byte(~Hzk[index].dat[t], OLED_DATA);
			}
			OLED_Set_Pos(x, y + 1);	 // 设置OLED光标位置
			for (t = 0; t < 16; t++) // 再写汉字下半部分数据
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
* @函数名：         OLED_ShowText
* @函数描述：       在指定位置显示中英文字符串
* @函数作者：       矛盾聚合体
* @输入参数：
					参数名    参数类型  参数描述
					@x：      u8         列坐标，0~127
					@y：      u8         行坐标，0~7
					@str：    u8*        要显示的字符串
					@flag：   u8         反白标志，非0时反白显示
* @返回值：         void
* @其他：
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
			if (*str & 0x80) // 判断下一个字符是中文还是英文
			{
				if (x >= 112)
				{
					y++;
					y++;
					x = 0;
				} // 修改地址
			}
			else
			{
				if (x >= 120)
				{
					y++;
					y++;
					x = 0;
				} // 修改地址
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
				} // 修改地址
			}
			else
			{
				if (x >= 120)
				{
					y++;
					y++;
					x = 0;
				} // 修改地址
			}
		}
	}
}
// 初始化SSD1306
void OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};			   // 定义GPIO结构体
	RCC_APB2PeriphClockCmd(IIC_OLED_SDA_GPIO_CLK, ENABLE); // 开启GPIO模块的时钟
	RCC_APB2PeriphClockCmd(IIC_OLED_SCL_GPIO_CLK, ENABLE); // 开启GPIO模块的时钟

	GPIO_InitStructure.GPIO_Pin = IIC_OLED_SDA_Pin;	   // 配置SDA端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 设置GPIO的模式为输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 设置端口输出类型为：推挽输出
	GPIO_Init(IIC_OLED_SDA_GPIO, &GPIO_InitStructure); // 初始化GPIO为高速推挽输出模式

	GPIO_InitStructure.GPIO_Pin = IIC_OLED_SCL_Pin;	   // 配置SCL端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 设置GPIO的模式为输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 配置 I/O 口的速度为：50MHz，还可选：GPIO_Speed_2MHz，GPIO_Speed_10MHz
	GPIO_Init(IIC_OLED_SCL_GPIO, &GPIO_InitStructure); // 初始化GPIO为高速推挽输出模式

	DelayMs(200);

	OLED_WR_Byte(0xAE, OLED_CMD); //关闭显示
	OLED_WR_Byte(0x00, OLED_CMD); //设置低列地址
	OLED_WR_Byte(0x10, OLED_CMD); //设置高列地址
	OLED_WR_Byte(0x40, OLED_CMD); //设置起始行地址,集映射RAM显示起始行(0x00~0x3F)
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

	OLED_WR_Byte(0x8D, OLED_CMD);//设置充电泵启用/禁用
	OLED_WR_Byte(0x14, OLED_CMD);//设置(0x10禁用,0x14启用)

	OLED_WR_Byte(0xAF, OLED_CMD); ////开启显示
	OLED_Clear();
}
void Display_Menu()
{
	OLED_ShowText(0,0,(u8*)"灯状态: 未开",0);
	OLED_ShowText(0,2,(u8*)"温度: --  度",0);
	OLED_ShowText(0,4,(u8*)"湿度: --  %RH",0);
	OLED_ShowText(0,6,(u8*)"服务器状态异常",0);
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

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
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
