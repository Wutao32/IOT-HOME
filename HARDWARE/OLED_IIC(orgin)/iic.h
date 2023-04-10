#ifndef __IIC_H
#define __IIC_H
#include "delay.h"
#include "sys.h"
#include "stdio.h"
#include "string.h"

#define write 0
#define read 1

// IIC总线地址接口定义
#define SCL PAout(0)
#define SDA_OUT PAout(1)
#define SDA_IN PAin(1)
//PA1配置为输入模式
#define IIC_INPUT_MODE_SET()      \
    {                             \
        GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1); \
        GPIOA->CRL |= GPIO_CRL_CNF5_1; \
    }
//PA1配置为输出模式 50MHZ 推挽输出
#define IIC_OUTPUT_MODE_SET()     \
    {                             \
        GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1); \
        GPIOA->CRL |= GPIO_CRL_MODE1; \
    }
void IIC_Init(void);
void IIC_ACK(void);
void IIC_NACK(void);
u8 IIC_wait_ACK(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(u8 byte);
u8 IIC_RcvByte(void);
#endif
