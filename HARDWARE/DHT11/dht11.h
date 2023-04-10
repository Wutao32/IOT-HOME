
#ifndef __DHT11_H
#define __DHT11_H

// PA5配置为输入模式
#define DHT11_IO_IN()                                    \
    {                                                    \
        GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5); \
        GPIOA->CRL |= GPIO_CRL_CNF5_1;                   \
    }
// PA5配置为输出模式，速率为50MHz，输出类型为推挽输出
#define DHT11_IO_OUT()                                   \
    {                                                    \
        GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5); \
        GPIOA->CRL |= GPIO_CRL_MODE5;                    \
    }
#define DHT11_OUT(x) GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)x) // PA5控制DHT11
#define DHT11_DQ_IN GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)        // PA5控制DHT11

void DHT11_Rst(void);
char DHT11_Check(void);
char DHT11_Read_Bit(void);
char DHT11_Read_Byte(void);
char DHT11_Read_Data(char *temp, char *humi);
char DHT11_Init(void);

#endif
