/*-------------------------------------------------*/
/*                                                 */
/*                 ���ƶ˿�           		   	   */
/*                                                 */
/*-------------------------------------------------*/

// LED�������ݷ��͸�������

#include "stm32f10x.h"
#include "pushdata.h"
#include "usart1.h"
#include "mqtt.h"
#include "led.h"
#include "dht11.h"				   //������Ҫ��ͷ�ļ�
#include "oled.h"
// extern enum ENUM_Switch LedFlag;   // �����״̬
// extern enum ENUM_Switch Dht11Flag; // ��ʪ�ȴ�����״̬
static char humidity;					   // ����һ������������ʪ��ֵ
static char temperature;				   // ����һ�������������¶�ֵ
static char tempAll[100];				   // ����һ����ʱ��������������������
static int dataLen; // ���ĳ���
/*-------------------------------------------------*/
/*�����������Ϳ����豸����                          */
/*��  ������                                       */
/*����ֵ��							               */
/*-------------------------------------------------*/
void Send_Data(Enum_Pub_Typedef type)
{

	//char tempAll[100];				   // ����һ����ʱ��������������������
	memset(tempAll, 0, 100); // ��ջ�����
	switch (type)
	{
	case LED:
		if (LED_GetStatus())
		{
			sprintf(tempAll, "%s", "on"); // ��������			
		}
		else
		{
			sprintf(tempAll, "%s", "off"); // ��������
			//OLED_DisplayString(16*3,0,16,16,"��");
		}
		dataLen = strlen(tempAll);
		u1_printf("%s\r\n", tempAll);
		MQTT_PublishQs0(P_TOPIC_LAMP, tempAll, dataLen); // ������ݣ�������������
		break;
	case DHT11:
		DHT11_Read_Data(&temperature, &humidity);														  // ��ȡ��ʪ��ֵ
		//sprintf(tempAll, "#%d%d#%d%d", temperature / 10, temperature % 10, humidity / 10, humidity % 10); // ��������
		sprintf(tempAll,"#%d#%d",temperature,humidity);
		//u1_printf("debugT:%d\r\n",temperature);
		//u1_printf("debugH:%d\r\n",humidity);
		u1_printf("%s\r\n", tempAll);
		//OLED_ShowText(16*3,2,(u8*)tempT,0);
		//OLED_ShowText(16*3,4,(u8*)tempH,0);
		OLED_ShowNumber(16*3,2,(u16)temperature,2,16);
		OLED_ShowNumber(16*3,4,(u16)humidity,2,16);
		
		// ������ʾ�������
		dataLen = strlen(tempAll);
		MQTT_PublishQs0(P_TOPIC_DHT, tempAll, dataLen); // ������ݣ�������������
	default:
		break;
	}
}
