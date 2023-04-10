 
/*-------------------------------------------------*/
/*         	                                       */
/*           	 LED（补光灯）控制           	    */
/*                                                 */
/*-------------------------------------------------*/

// 硬件连接：
// PC13  LED0

#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件
#define LED_APB_GPIO  	RCC_APB2Periph_GPIOC
#define LED_Pin		GPIO_Pin_13	
#define LED_GPIO       GPIOC
/*-------------------------------------------------*/
/*函数名：初始化LED函数                       	    */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;						//定义GPIO结构体	
	RCC_APB2PeriphClockCmd(LED_APB_GPIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED_Pin;						//配置LED端口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;				//配置端口的速度	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;				//配置端口模式为推挽输出模式
	GPIO_Init(LED_GPIO,&GPIO_InitStructure);					//对端口进行初始化	
	LED_Off();
}

/*-------------------------------------------------*/
/*函数名：LED开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_On(void)
{			
	GPIO_SetBits(GPIOC, GPIO_Pin_13); 						 //PD2 输出高
} 


/*-------------------------------------------------*/
/*函数名：LED关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{		
	GPIO_ResetBits(GPIOC, GPIO_Pin_13); 						 //PD2 输出低
}

//LED反转
void LED_Reverse(void)
{
	if(GPIO_ReadOutputDataBit(LED_GPIO,LED_Pin))
	GPIO_ResetBits(LED_GPIO,LED_Pin);
	else
	GPIO_SetBits(LED_GPIO,LED_Pin);
}	
//获取LED状态
char LED_GetStatus(void)
{
	return GPIO_ReadOutputDataBit(LED_GPIO,LED_Pin)?1:0;
}
