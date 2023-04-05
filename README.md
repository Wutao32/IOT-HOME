# IOT-HOME

> 基于wifi和云平台的智能家居系统

## 本项目是基于stm3f103c8t6的智能家居系统

使用esp8266和巴法云平台进行通信，对传感器数据进行采集和控制

=======

### 引脚接线

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
| EN      | PA5            |

