
#ifndef __MQTT_H
#define __MQTT_H
#include "main.h"
#include "usart.h"
#include "common.h"

#define R_NUM 5        // 接收缓冲区个数
#define RBUFF_UNIT 300 // 接收缓冲区长度

#define T_NUM 5        // 发送缓冲区个数
#define TBUFF_UNIT 300 // 发送缓冲区长度

#define C_NUM 5        // 命令缓冲区个数
#define CBUFF_UNIT 300 // 命令缓冲区长度

#define MQTT_USARTx USART2
#define MQTT_TxData(fmt, ...) USART_printf(MQTT_USARTx, fmt, ##__VA_ARGS__) // 串口2负责数据发送

#define PRODUCTID "esp8266"                       // 产品ID,Username
#define PRODUCTID_LEN strlen(PRODUCTID)           // 产品ID长度
#define DEVICEID "mqttx_00"                       // 设备ID ,ClientID
#define DEVICEID_LEN strlen(DEVICEID)             // 长度
#define AUTHENTICATION "esp8266"                  // 鉴权信息/密码,Passward
#define AUTHENTICATION_LEN strlen(AUTHENTICATION) // 鉴权信息长度/密码长度
#define S_TOPIC_NAME "topic_con"                  // 订阅的主题
#define P_TOPIC_NAME "topic_two"                  // 发布的主题
#define MQTTServerIP "www.biadu.com"            // MQTT服务器IP
#define MQTTServerPort "80"                       //MQTT服务端口

extern unsigned char MQTT_RxDataBuf[R_NUM][RBUFF_UNIT]; // 外部变量声明，数据的接收缓冲区,所有服务器发来的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
extern unsigned char *MQTT_RxDataInPtr;                 // 外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_RxDataOutPtr;                // 外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_RxDataEndPtr;                // 外部变量声明，指向缓冲区结束的位置
extern unsigned char MQTT_TxDataBuf[T_NUM][TBUFF_UNIT]; // 外部变量声明，数据的发送缓冲区,所有发往服务器的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
extern unsigned char *MQTT_TxDataInPtr;                 // 外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_TxDataOutPtr;                // 外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_TxDataEndPtr;                // 外部变量声明，指向缓冲区结束的位置
extern unsigned char MQTT_CMDBuf[C_NUM][CBUFF_UNIT];    // 外部变量声明，命令数据的接收缓冲区
extern unsigned char *MQTT_CMDInPtr;                    // 外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_CMDOutPtr;                   // 外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_CMDEndPtr;                   // 外部变量声明，指向缓冲区结束的位置

extern char ClientID[64]; // 外部变量声明，存放客户端ID的缓冲区
extern int ClientID_len;   // 外部变量声明，存放客户端ID的长度
extern char Username[64]; // 外部变量声明，存放用户名的缓冲区
extern int Username_len;   // 外部变量声明，存放用户名的长度
extern char Passward[64]; // 外部变量声明，存放密码的缓冲区
extern int Passward_len;   // 外部变量声明，存放密码的长度
extern char ServerIP[64]; // 外部变量声明，存放服务器IP或是域名
extern char ServerPort[64];     // 外部变量声明，存放服务器的端口号 

extern char pingFlag; // 外部变量声明，ping报文状态      0：正常状态，等待计时时间到，发送Ping报文
                      // 外部变量声明，ping报文状态      1：Ping报文已发送，当收到 服务器回复报文的后 将1置为0
extern char connectFlag;      // 外部变量声明，同服务器连接状态  0：还没有连接服务器  1：连接上服务器了
extern char reConnectFlag;    // 外部变量声明，重连服务器状态    0：连接还存在  1：连接断开，重连
extern char connectPackFlag;  // 外部变量声明，CONNECT报文状态   1：CONNECT报文成功
extern char subcribePackFlag; // 外部变量声明，订阅报文状态      1：订阅报文成功

void MQTT_Buff_Init(void);
void IoT_Parameter_Init(void);
void MQTT_ConectPack(void);
void MQTT_Subscribe(char *, int);
void MQTT_PingREQ(void);
void MQTT_PublishQs0(char *, char *, int);
void MQTT_DealPushdata_Qs0(unsigned char *);
void TxDataBuf_Deal(unsigned char *, int);
void CMDBuf_Deal(unsigned char *, int);

#endif
