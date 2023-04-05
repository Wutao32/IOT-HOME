
#ifndef __COMMON_H
#define __COMMON_H

// #include "stm32f4xx.h"
#include "main.h"

/********************************** 函数声明 ***************************************/
//格式化串口输出 LL库
void USART_printf(USART_TypeDef *USARTx, char *Data, ...);

#endif /* __COMMON_H */
