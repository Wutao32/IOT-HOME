 /*-----------------------------------------------------*/
/*                                                     */
/*            ����main��������ں���Դ�ļ�              */
/*                                                     */
/*-----------------------------------------------------*/
//#define DEBUG
#if defined DEBUG
#include "stm32f10x.h"
#include "delay.h"
#include "usart1.h"
#include "dht11.h"
#include "oled.h"
#include "led.h"
char *ledFlag = "off";       //LED״̬
char *dhtFlag = "off";		        //��ʪ�����ݴ���״̬
char humidity;					   // ����һ������������ʪ��ֵ
char temperature;				   // ����һ�������������¶�ֵ
char tempH[10];						//��ʱ������������ʪ��
char tempT[10];						//��ʱ�������������¶�
int main()
{

	Delay_Init();          //��ʱ���ܳ�ʼ��              
	//Usart1_Init(115200);     //����1���ܳ�ʼ����������115200
	DHT11_Init();          //��ʼ��DHT11
	LED_Init();
	OLED_Init();
	//OLED_DisplayString(0,0,16,16,"��ʪ�ȷ���������״̬�ƿ������ӳɹ�ʧ��");
	//OLED_Menu_Display();
	Display_Menu();
	while(1){
		//DHT11_Read_Data(&temperature, &humidity);
		//u1_printf("#%d%d#%d%d\r\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
		LED_Reverse();
		DHT11_Read_Data(&temperature, &humidity);														  // ��ȡ��ʪ��ֵ
		memset(tempT,0,10);
		memset(tempH,0,10);
		sprintf(tempT,"%d%d",temperature / 10, temperature % 10);
		sprintf(tempH,"%d%d",humidity / 10, humidity % 10);
																				  // ��ջ�����2
		//sprintf(tempAll, "#%d%d#%d%d", temperature / 10, temperature % 10, humidity / 10, humidity % 10); // ��������
		
		OLED_ShowText(16*3,2,(u8*)tempT,0);
		OLED_ShowText(16*3,4,(u8*)tempH,0);																	  // ������ʾ�������
		//if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13))
			//OLED_DisplayString(16*3,0,16,16,"��");
		//else
			//OLED_DisplayString(16*3,0,16,16,"��");
		DelayMs(1000);
	}
//	return 0;
}
#else
#include "stm32f10x.h"   //������Ҫ��ͷ�ļ�
#include "pushdata.h"     //������Ҫ��ͷ�ļ�
#include "delay.h"       //������Ҫ��ͷ�ļ�
#include "usart1.h"      //������Ҫ��ͷ�ļ�
#include "usart2.h"      //������Ҫ��ͷ�ļ�
#include "timer2.h"      //������Ҫ��ͷ�ļ�
#include "timer3.h"      //������Ҫ��ͷ�ļ�
#include "timer4.h"      //������Ҫ��ͷ�ļ�
#include "wifi.h"	     //������Ҫ��ͷ�ļ�
#include "led.h"	     //������Ҫ��ͷ�ļ� LED                                                  
#include "mqtt.h"        //������Ҫ��ͷ�ļ�
#include "dht11.h"       //������Ҫ��ͷ�ļ� ������ʪ��
#include "oled.h"

char *cmdLED_On = "LAMP002on";      //LED��
char *cmdLED_Off = "LAMP002off";    //LED�ر�

char *cmdDHT11_On = "DHTCONTROLon";    //��ʪ�����ݴ����
char *cmdDHT11_Off = "DHTCONTROLoff";  //��ʪ�����ݴ���ر�

int main(void) 
{	
	Delay_Init();          //��ʱ���ܳ�ʼ��              
	Usart1_Init(115200);     //����1���ܳ�ʼ����������115200
	Usart2_Init(115200);   //����2���ܳ�ʼ����������115200	
	TIM4_Init(500,7200);   //TIM4��ʼ������ʱʱ�� 500*7200*1000/72000000 = 50ms
	LED_Init();			   //LED��ʼ��
	DHT11_Init();          //��ʼ��DHT11	
	OLED_Init();				//��ʼ��OLED
	Display_Menu();
	WiFi_ResetIO_Init();   //��ʼ��WiFi�ĸ�λIO
	IoT_Parameter_Init();  //��ʼ����IoTƽ̨MQTT�������Ĳ���	
	
	while(1)
	{		
		// connectFlag=1ͬ���������������� 
		if(connectFlag == 1) 
		{   
			OLED_ShowText(0,6,(u8*)"������״̬����",0);  
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������						 */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr) //if�����Ļ���˵�����ͻ�������������
			{                
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���connectPackFlag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�subcribePackFlag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2] == 0x10)||((MQTT_TxDataOutPtr[2] == 0x82)&&(connectPackFlag == 1))||(subcribePackFlag == 1)) 
				{    
					if (MQTT_TxDataOutPtr[2] == 0x30) 
					{	
						u1_printf("��������:0x%x����Ƭ������������������\r\n", MQTT_TxDataOutPtr[2]);	
						u1_printf("\r\n");
					}
					else 
					{
						u1_printf("��������:0x%x\r\n", MQTT_TxDataOutPtr[2]);//������ʾ��Ϣ
					}
					
					MQTT_TxData(MQTT_TxDataOutPtr);							//��������
					MQTT_TxDataOutPtr += TBUFF_UNIT;						//ָ������
					if(MQTT_TxDataOutPtr == MQTT_TxDataEndPtr)				//���ָ�뵽������β����
					{ 
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];				//ָ���λ����������ͷ
					}	
				} 				
			}
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr) //if�����Ļ���˵�����ջ�������������	
			{												
				u1_printf("���յ�����:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(  0x20==MQTT_RxDataOutPtr[2])
				{             			
					switch(MQTT_RxDataOutPtr[5])
					{					   
						case 0x00 : u1_printf("CONNECT���ĳɹ�\r\n");							//���������Ϣ	
										connectPackFlag = 1;									//CONNECT���ĳɹ������ı��Ŀɷ�
									break;														//������֧case 0x00                                              
						case 0x01 : u1_printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");    //���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������
									break;														//������֧case 0x01   
						case 0x02 : u1_printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");//���������Ϣ
									connectFlag = 0;                                            //connectFlag���㣬��������
									break; 														//������֧case 0x02 
						case 0x03 : u1_printf("�����Ѿܾ�������˲����ã�׼������\r\n");	    //���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������
									break;														//������֧case 0x03
						case 0x04 : u1_printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");	//���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������						
									break;														//������֧case 0x04
						case 0x05 : u1_printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");				//���������Ϣ
									connectFlag = 0;											//connectFlag���㣬��������						
									break;														//������֧case 0x05 		
						default   : u1_printf("�����Ѿܾ���δ֪״̬��׼������\r\n");		    //���������Ϣ 
									connectFlag = 0;											//connectFlag���㣬��������					
									break;														//������֧case default 								
					}				
			    }			
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(0x90 == MQTT_RxDataOutPtr[2])
				{ 
					switch(MQTT_RxDataOutPtr[6])
					{					
						case 0x00 :
						case 0x01 : 
									u1_printf("���ĳɹ�\r\n");			//���������Ϣ
									subcribePackFlag = 1;				//subcribePackFlag��1����ʾ���ı��ĳɹ����������Ŀɷ���
									pingFlag = 0;						//pingFlag����
									TIM3_ENABLE_30S();					//����30s��PING��ʱ��	
									//Send_Data();
									TIM2_ENABLE_10S();
									break;								//������֧                                             
						default: 
									u1_printf("����ʧ�ܣ�׼������\r\n");//���������Ϣ 
									connectFlag = 0;					//connectFlag���㣬��������
									break;								//������֧ 								
						}					
			    }
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(0xD0 == MQTT_RxDataOutPtr[2] )
				{ 
					u1_printf("PING���Ļظ�\r\n");                      //���������Ϣ 
					if(pingFlag == 1)
					{                   						        //���pingFlag=1����ʾ��һ�η���
						 pingFlag = 0;    				       			//Ҫ���pingFlag��־
					}
					else if(pingFlag > 1)	
					{ 				 									//���pingFlag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						pingFlag = 0;     				      			//Ҫ���pingFlag��־
						TIM3_ENABLE_30S(); 				      			//PING��ʱ���ػ�30s��ʱ��
					}				
				}	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if((0x30==MQTT_RxDataOutPtr[2]))
			    { 
					u1_printf("�������ȼ�0����\r\n"); 		   	//���������Ϣ
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);   //����ȼ�0��������
				}									
				MQTT_RxDataOutPtr += RBUFF_UNIT;                //ָ������
				if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)      //���ָ�뵽������β����
				{
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];      //ָ���λ����������ͷ              
				}		          
			}//������ջ��������ݵ�else if��֧��β
						
			/*---------------------------------------------------------------------------------------------------------------------*/
			/*                    							 ���������������                   		          			         */
			/*---------------------------------------------------------------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr)							  //if�����Ļ���˵�����������������	
			{                             		       
				u1_printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);              //���������Ϣ
				
				if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_On,strlen(cmdLED_On)))//�ж�ָ������CMD1 
				{                                            
					//LedFlag = ON;  								             
					LED_On();
					Send_Data(LED);
					OLED_ShowText(0,0,(u8*)"��״̬: �ѿ�",0);							  		   //LED����
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdLED_Off,strlen(cmdLED_Off))) //�ж�ָ������CMD11
				{ 
					//LedFlag = OFF;                                              
					LED_Off(); 	
					Send_Data(LED);
					OLED_ShowText(0,0,(u8*)"��״̬: δ��",0);							                //LED�ر�
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdDHT11_On,strlen(cmdDHT11_On)))//�ж�ָ������CMD3
				{      
					// Dht11Flag = ON;                 //dataFlag��1����ʾ���ڲɼ�״̬��
					TIM2_ENABLE_10S();           //��ʱ��2��,10s����ɼ���ʪ��   
				}
				else if(!memcmp(&MQTT_CMDOutPtr[2],cmdDHT11_Off,strlen(cmdDHT11_Off)))//�ж�ָ������CMD3
				{     
					//Dht11Flag = OFF;                 //dataFlag��0����ʾ����ֹͣ״̬��
					TIM_Cmd(TIM2,DISABLE);		 //�ж�2·����״̬�Ͳɼ�״̬����������������
				}								
				//������������ᷢ��״̬
				else u1_printf("δָ֪��\r\n");				//���������Ϣ	
			
				MQTT_CMDOutPtr += CBUFF_UNIT;				//ָ������
				if(MQTT_CMDOutPtr == MQTT_CMDEndPtr)	    //���ָ�뵽������β����
				MQTT_CMDOutPtr = MQTT_CMDBuf[0];			//ָ���λ����������ͷ	
				
				//Send_Data();//���Ϳ�������
				//��������������ݵ�else if��֧��β					
			}	//connectFlag=1��if��֧�Ľ�β
		}
	/*--------------------------------------------------------------------*/
	/*      connectFlag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����            */
	/*--------------------------------------------------------------------*/
		else
		{
			OLED_ShowText(0,6,(u8*)"������״̬�쳣",0);
			u1_printf("��Ҫ���ӷ�����\r\n");               //���������Ϣ
			TIM_Cmd(TIM4, DISABLE);                        //�ر�TIM4 
			TIM_Cmd(TIM3, DISABLE);                        //�ر�TIM3  
			WiFi_RxCounter = 0;                            //WiFi������������������                        
			memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);      //���WiFi���ջ����� 
			if(WiFi_Connect_IoTServer() == 0)			   //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
			{   			     
				u1_printf("����TCP���ӳɹ�\r\n");          //���������Ϣ
				connectFlag = 1;                           //connectFlag��1����ʾ���ӳɹ�	
				WiFi_RxCounter = 0;                        //WiFi������������������                        
				memset(WiFi_RX_BUF, 0, WiFi_RXBUFF_SIZE);  //���WiFi���ջ����� 
				MQTT_Buff_Init();                          //��ʼ�����ͻ�����                    
			}				
		}
	}
}
#endif
