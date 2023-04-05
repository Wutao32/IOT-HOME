/*
 * @Author: Wu Tao wt20010827@gmail.com
 * @Date: 2023-03-31 23:20:13
 * @LastEditors: Wu Tao
 * @LastEditTime: 2023-04-03 19:51:51
 * @Description: file content
 * @FilePath: \MDK-ARMd:\Codefiles\STM32\F103\C8T6\TEMP\Core\HAL_ESP8266_Drive\ESP8266\bsp_esp8266_test.c
 * Copyright (c) 2023 by 吴涛, All Rights Reserved.
 */
#include "bsp_esp8266_test.h"
#include "stdio.h"
volatile uint8_t ucTcpClosedFlag = 0;
#define LED_ON() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET)
#define LED_OFF() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET)
/******************************************************************
 *    !!!!! 注意：ESP8266 接收到的数据存储在数组 strEsp8266_Fram_Record .Data_RX_BUF 中，
 *               需要解析数据，并做出对应的操作
 ******************************************************************/

/********************************************连接服务器*************************************************************************/

/**
 * @brief  ESP8266 StaTcpClient Unvarnish 配置测试函数
 * @param  无
 * @retval 无
 */
void ESP8266_StaTcpClient_Unvarnish_ConfigTest(void)
{
  printf("准备配置ESP8266......\r\n");
  printf("使能ESP8266......\r\n");
  ESP8266_CH_ENABLE();
  while (!ESP8266_AT_Test())
  {
  };

  printf("准备配置工作模式: STA......\r\n");
  while (!ESP8266_Net_Mode_Choose(STA))
  {
  };

  printf("准备连接路由器......\r\n");
  while (!ESP8266_JoinAP(User_ESP8266_ApSsid, User_ESP8266_ApPwd))
  {
  };

  printf("准备关闭多连接......\r\n");
	while (!ESP8266_Enable_MultipleId(DISABLE))
  {
  };

  printf("准备连接服务器 ......\r\n");
  while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TcpServer_IP, User_ESP8266_TcpServer_Port, Single_ID_0))
  {
  };
  printf("连接服务器成功 ......\r\n");
  printf("准备进入透传模式 ......\r\n");
  while (!ESP8266_UnvarnishSend())
  {
  };
	printf("进入透传模式成功......\r\n");
  printf("配置 ESP8266 完毕\r\n");
}

/*************************************************************************************************************************/

/**
 * @brief  ESP8266 检查是否接收到了数据，检查连接和掉线重连
 * @param  无
 * @retval 无
 */
void ESP8266_CheckRecvDataTest(void)
{
  uint8_t ucStatus;

  if (strEsp8266_Fram_Record.InfBit.FramFinishFlag) // 接收到数据
  {
    // USART_ITConfig ( ESP8266_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
    LL_USART_DisableIT_RXNE(USART2);
    strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';

    // 使用USART1将esp8266接收到的数据通过串口调试助手打印出来
    printf("%s\n", strEsp8266_Fram_Record.Data_RX_BUF);
    if(strstr(strEsp8266_Fram_Record.Data_RX_BUF,"LEDON")){
      LED_ON();
    }else if(strstr(strEsp8266_Fram_Record.Data_RX_BUF,"LEDOFF")){
      LED_OFF();
    }
    // 解析数据后，执行的操作函数在这里调用

    strEsp8266_Fram_Record.InfBit.FramLength = 0;     // 接收数据长度置零
    strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0; // 接收标志置零
    LL_USART_EnableIT_RXNE(USART2);                   // 使能串口接收中断
  }

  if (ucTcpClosedFlag) // 检测是否失去连接
  {
    ESP8266_ExitUnvarnishSend(); // 退出透传模式

    do
      ucStatus = ESP8266_Get_LinkStatus(); // 获取连接状态
    while (!ucStatus);

    if (ucStatus == 4) // 确认失去连接后重连
    {
      // 正在重连热点和服务器
      printf("\r\nReconnecting to hotspot and server ......\r\n");

      while (!ESP8266_JoinAP(User_ESP8266_ApSsid, User_ESP8266_ApPwd))
        ;

      while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TcpServer_IP, User_ESP8266_TcpServer_Port, Single_ID_0))
        ;

      // 重连热点和服务器成功
      printf("\r\nReconnect to hotspot and server successfully\r\n");
    }

    while (!ESP8266_UnvarnishSend())
      ;
  }
}
