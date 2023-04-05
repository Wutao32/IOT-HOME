
#ifndef  __ESP8266_TEST_H
#define	 __ESP8266_TEST_H



#include "main.h"
/********************************** 头文件包含区 **********************************/

#include "esp8266.h"
#include "stdio.h"  
#include <string.h>  
#include <stdbool.h>
#include "usart.h"
/************************************* END *****************************************/


/********************************** 用户需要设置的参数**********************************/
#define      User_ESP8266_ApSsid                      "wutao"        //要连接的热点的名称
#define      User_ESP8266_ApPwd                       "12345678"         //要连接的热点的密钥

#define      User_ESP8266_TcpServer_IP                "192.168.137.145"    //要连接的服务器的 IP
#define      User_ESP8266_TcpServer_Port              "8080"              //要连接的服务器的端口


/********************************** 外部全局变量 ***************************************/
extern volatile uint8_t ucTcpClosedFlag;



/********************************** 测试函数声明 ***************************************/
void ESP8266_StaTcpClient_Unvarnish_ConfigTest(void);

void ESP8266_CheckRecvDataTest(void);

#endif

