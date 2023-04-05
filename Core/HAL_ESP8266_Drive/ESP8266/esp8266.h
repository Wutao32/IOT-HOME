#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "main.h"
#include "common.h"
#include <stdio.h>
#include <stdbool.h>
#include "usart.h"
#include <string.h>

#if defined(__CC_ARM)
#pragma anon_unions
#endif
/**
 * 引脚定义
 *RST	PA4
 *EN	PA5
 *TX	PA3(USART2-RX)
 *RX	PA2(USART2-TX)
 */
/******************************** ESP8266 使用的USARTx ***********************************/
#define ESP8266_USARTx USART2
//WIFI参数，用户定义
#define      SSID                      "wutao"        //要连接的热点的名称
#define      PASS                       "12345678"         //要连接的热点的密钥
extern char  ServerIP[64];
extern char  ServerPort[64]; 
//#define      User_ESP8266_TcpServer_IP                "192.168.137.145"    //要连接的服务器的 IP
//#define      User_ESP8266_TcpServer_Port              "8080"              //要连接的服务器的端口
/******************************* ESP8266 数据类型定义 ***************************/
//ESP8266模式选择
typedef enum
{
	STA,
	AP,
	STA_AP
} ENUM_Net_ModeTypeDef;
//传输协议选择
typedef enum
{
	enumTCP,
	enumUDP,
} ENUM_NetPro_TypeDef;
//连接号，通过指定连接号防止其他计算机访问同一端口引发错误
typedef enum
{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
//加密模式
typedef enum
{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;

/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN 1024 // 最大接收缓存字节数

extern struct STRUCT_USARTx_Fram // 串口数据帧的处理结构体
{
	char Data_RX_BUF[RX_BUF_MAX_LEN];

	union
	{
		__IO uint16_t InfAll;
		struct
		{
			__IO uint16_t FramLength : 15;	  // 14:0
			__IO uint16_t FramFinishFlag : 1; // 15
		} InfBit;
	};

} strEsp8266_Fram_Record;

extern struct STRUCT_USARTx_Fram strUSART_Fram_Record;

/*********************************************** ESP8266 函数宏定义 *******************************************/
#define ESP8266_Usart(fmt, ...) USART_printf(ESP8266_USARTx, fmt, ##__VA_ARGS__)
#define PC_Usart(fmt, ...) printf(fmt, ##__VA_ARGS__)

#define ESP8266_CH_ENABLE() HAL_GPIO_WritePin(ESP8266_EN_GPIO_Port, ESP8266_EN_Pin, GPIO_PIN_SET)
#define ESP8266_CH_DISABLE() HAL_GPIO_WritePin(ESP8266_EN_GPIO_Port, ESP8266_EN_Pin, GPIO_PIN_RESET)

#define ESP8266_RST_HIGH_LEVEL() HAL_GPIO_WritePin(ESP8266_RST_GPIO_Port, ESP8266_RST_Pin, GPIO_PIN_SET)
#define ESP8266_RST_LOW_LEVEL() HAL_GPIO_WritePin(ESP8266_RST_GPIO_Port, ESP8266_RST_Pin, GPIO_PIN_RESET)

/****************************************** ESP8266 函数声明 ***********************************************/
void ESP8266_Init(void);
void ESP8266_Rst(void);
bool ESP8266_Cmd(char *cmd, char *reply1, char *reply2, uint32_t waittime);
bool ESP8266_AT_Test(void);
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode);
bool ESP8266_JoinAP(char *pSSID, char *pPassWord);
bool ESP8266_BuildAP(char *pSSID, char *pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode);
bool ESP8266_Enable_MultipleId(FunctionalState enumEnUnvarnishTx);
bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char *ip, char *ComNum, ENUM_ID_NO_TypeDef id);
bool ESP8266_StartOrShutServer(FunctionalState enumMode, char *pPortNum, char *pTimeOver);
uint8_t ESP8266_Get_LinkStatus(void);
uint8_t ESP8266_Get_IdLinkStatus(void);
uint8_t ESP8266_Inquire_ApIp(char *pApIp, uint8_t ucArrayLength);
bool ESP8266_UnvarnishSend(void);
void ESP8266_ExitUnvarnishSend(void);
bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char *pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId);
char *ESP8266_ReceiveString(FunctionalState enumEnUnvarnishTx);
char WiFi_Connect_IoTServer(void);
#endif
