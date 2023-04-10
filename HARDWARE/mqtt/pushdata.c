/*-------------------------------------------------*/
/*                                                 */
/*                 控制端口           		   	   */
/*                                                 */
/*-------------------------------------------------*/

// LED控制数据发送给服务器

#include "stm32f10x.h"
#include "pushdata.h"
#include "usart1.h"
#include "mqtt.h"
#include "led.h"
#include "dht11.h"				   //包含需要的头文件
#include "oled.h"
// extern enum ENUM_Switch LedFlag;   // 补光灯状态
// extern enum ENUM_Switch Dht11Flag; // 温湿度传感器状态
static char humidity;					   // 定义一个变量，保存湿度值
static char temperature;				   // 定义一个变量，保存温度值
static char tempAll[100];				   // 定义一个临时缓冲区，包括所有数据
static int dataLen; // 报文长度
/*-------------------------------------------------*/
/*函数名：发送控制设备数据                          */
/*参  数：无                                       */
/*返回值：							               */
/*-------------------------------------------------*/
void Send_Data(Enum_Pub_Typedef type)
{

	//char tempAll[100];				   // 定义一个临时缓冲区，包括所有数据
	memset(tempAll, 0, 100); // 清空缓冲区
	switch (type)
	{
	case LED:
		if (LED_GetStatus())
		{
			sprintf(tempAll, "%s", "on"); // 构建报文			
		}
		else
		{
			sprintf(tempAll, "%s", "off"); // 构建报文
			//OLED_DisplayString(16*3,0,16,16,"关");
		}
		dataLen = strlen(tempAll);
		u1_printf("%s\r\n", tempAll);
		MQTT_PublishQs0(P_TOPIC_LAMP, tempAll, dataLen); // 添加数据，发布给服务器
		break;
	case DHT11:
		DHT11_Read_Data(&temperature, &humidity);														  // 读取温湿度值
		//sprintf(tempAll, "#%d%d#%d%d", temperature / 10, temperature % 10, humidity / 10, humidity % 10); // 构建报文
		sprintf(tempAll,"#%d#%d",temperature,humidity);
		//u1_printf("debugT:%d\r\n",temperature);
		//u1_printf("debugH:%d\r\n",humidity);
		u1_printf("%s\r\n", tempAll);
		//OLED_ShowText(16*3,2,(u8*)tempT,0);
		//OLED_ShowText(16*3,4,(u8*)tempH,0);
		OLED_ShowNumber(16*3,2,(u16)temperature,2,16);
		OLED_ShowNumber(16*3,4,(u16)humidity,2,16);
		
		// 串口显示相关数据
		dataLen = strlen(tempAll);
		MQTT_PublishQs0(P_TOPIC_DHT, tempAll, dataLen); // 添加数据，发布给服务器
	default:
		break;
	}
}
