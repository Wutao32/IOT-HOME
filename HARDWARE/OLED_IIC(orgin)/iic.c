#include "stm32f10x.h"
#include "iic.h"
 
void IIC_Init(void)
{
  /*1.开时钟*/
  RCC->APB2ENR |= 1<<2;
  /*2.设置GPIO模式*/
  GPIOA->CRL &= 0xFFFFFF00;
  GPIOA->CRL |= 0x00000033;
  /*3.设置GPIO空闲电平*/
  GPIOA->ODR |= 0x3<<0;	
  //printf("IIC_Init OK！\n");
}	
 
/*******************************************************************
功能:启动I2C总线,即发送I2C起始条件. SCL为高电平期间 SDA出现下降沿
********************************************************************/
void IIC_Start(void)
{
  IIC_OUTPUT_MODE_SET();
  SDA_OUT=1;
  SCL=1;
  SDA_OUT=0;
	SCL=0;
}
 
/*******************************************************************
功能:结束I2C总线,即发送I2C结束条件. SCL为高电平期间 SDA出现上升沿 
********************************************************************/
void IIC_Stop(void)
{
	IIC_OUTPUT_MODE_SET();
	SDA_OUT=0;  	
	SCL=1;
	SDA_OUT=1; 
}
 
/*主机发送ACK*/
void IIC_ACK(void)
{
  IIC_OUTPUT_MODE_SET();
  SCL=0; 
  SDA_OUT=0;   
  SCL=1;                
  SCL=0;                        
}
 
/*主机不发送ACK*/
void IIC_NACK(void)
{
  IIC_OUTPUT_MODE_SET();
  SCL=0;
  SDA_OUT=1;     
  SCL=1;                  
  SCL=0;                        
}
 
/*主机等待从机的ACK*/
u8 IIC_wait_ACK(void)
{
	u8 t = 200;
	IIC_OUTPUT_MODE_SET();
	SDA_OUT=1;//8位发送完后释放数据线，准备接收应答位 
	SCL=0;
	IIC_INPUT_MODE_SET();
	while(SDA_IN)//等待应答
	{
		t--;
		DelayUs(1); 
		if(t==0)
		{
			SCL=0;
			return 1;
		}
	}     
	SCL=1;
	SCL=0;                
	return 0;	
}
 
/*******************************************************************
字节数据发送函数               
函数原型: void  SendByte(UCHAR c);
功能:将数据c发送出去,可以是地址,也可以是数据
********************************************************************/
void  IIC_SendByte(u8 byte)
{
	u8  BitCnt;
	IIC_OUTPUT_MODE_SET();
	SCL=0;
	for(BitCnt=0;BitCnt<8;BitCnt++)//要传送的数据长度为8位
	{
		if(byte&0x80) SDA_OUT=1;//判断发送位
		else SDA_OUT=0; 
		byte<<=1;
		SCL=1;
		SCL=0;
	}
}
/*******************************************************************
 字节数据接收函数               
函数原型: UCHAR  RcvByte();
功能: 用来接收从器件传来的数据  
********************************************************************/    
u8 IIC_RcvByte(void)
{
  u8 retc;
  u8 BitCnt;
  retc=0; 
  IIC_INPUT_MODE_SET();//置数据线为输入方式                   
  for(BitCnt=0;BitCnt<8;BitCnt++)
  {  
		SCL=0;	//置时钟线为低，准备接收数据位
		DelayUs(1);               
		SCL=1;	//置时钟线为高使数据线上数据有效                
		retc=retc<<1;
		if(SDA_IN) retc |=1;//读数据位,接收的数据位放入retc中 
  }
  SCL=0;    
  return(retc);
}
