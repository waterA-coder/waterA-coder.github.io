#ifndef __DHT11_H
#define __DHT11_H 
#include "stm32f10x.h"

// DHT11 接 PA0
#define DHT11_GPIO_PORT    GPIOA
#define DHT11_GPIO_PIN     GPIO_Pin_0
#define DHT11_GPIO_CLK     RCC_APB2Periph_GPIOA

// 方向切换（用 GPIO_Init 重新配，清楚但稍慢）
void DHT11_IO_IN(void);
void DHT11_IO_OUT(void);

// 读写电平
#define DHT11_DQ_OUT(x)  GPIO_WriteBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN, (BitAction)(x))
#define DHT11_DQ_IN      GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

u8 DHT11_Init(void);
u8 DHT11_Read_Data(u8 *temp_H,u8 *temp_L,u8 *humi_H,u8 *humi_L);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Check(void);
void DHT11_Rst(void);

#endif













