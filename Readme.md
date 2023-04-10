# BemfaByMqtt

**简介：** ESP8266通过MQTT协议将传感器数据传输至巴法云平台，并通过云平台对设备进行控制

#### 接入介绍

[巴法开放平台 (bemfa.com)](https://cloud.bemfa.com/docs/#/?id=_111-%e6%8e%a5%e5%85%a5%e4%bb%8b%e7%bb%8d)

#### 引脚接线：

USART1(DEBUG):

| TTL-USB | STM32           |
| ------- | --------------- |
| GND     | GND             |
| VCC     | VCC             |
| TX      | PA10(USART1_RX) |
| RX      | PA9(USART1_TX)  |

USART2(ESP8266)

| ESP8266 | STM            |
| ------- | -------------- |
| GND     | GND            |
| VCC     | VCC            |
| TX      | PA3(USART2_RX) |
| RX      | PA2(USART2_TX) |
| RST     | PA4            |

DHT11

| DHT11 | STM32 |
| ----- | ----- |
| DATA  | PA5   |
| 3V3   | VCC   |
| GND   | GND   |

LED

| LED | STM32 |
| --- | ----- |
| VCC | PC13  |
| GND | GND   |

OLED

| OLED | STM32 |
| ---- | ----- |
| SCL  | PA0   |
| SDA  | PA1   |
| 3V3  | VCC   |
| GND  | GND   |

#### 定时器配置

| 定时器  | 功能                                                |
| ------- | --------------------------------------------------- |
| 定时器1 |                                                     |
| 定时器2 | 传感器数据处理(10s)                                 |
| 定时器3 | 心跳定时器（正常 30s ping 一次，快速 2s ping 一次） |
| 定时器4 | 处理MQTT数据(50ms)                                  |

#### 优先级分配

| 任务   | 优先级                   |
| ------ | ------------------------ |
| USART1 | 1,0                      |
| USART2 | 0,0                      |
| TIM2   | 2,1                      |
| TIM3   | normal:2,0<br />fast:1,0 |
| TIM4   | 1,0                      |

### 任务分配

| 外设   | 任务           |
| ------ | -------------- |
| USART1 | 输出调试信息   |
| USART2 | 配置ESP8266    |
| TIM2   | 传感器数据处理 |
| TIM3   | ping控制       |
| TIM4   | MQTT报文处理   |
