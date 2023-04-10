
#ifndef __CONTROL_H
#define __CONTROL_H


//发布类型
typedef enum {
    LED,
    DHT11
}Enum_Pub_Typedef;
//传感器开关状态
typedef enum {
	ON = 1,
	OFF = !ON
}ENUM_Switch;
void Send_Data(Enum_Pub_Typedef type);
#endif


