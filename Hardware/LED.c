#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2);
}

/**
  * @brief  控制LED开关
  * @param  LEDx: LED编号，可输入 LED1 或 LED2
  * @param  status: LED状态，可输入 ON 或 OFF
  * @retval 无
  */
void LEDx_ON(uint8_t LEDx, uint8_t status)
{
    // 定义LED对应的引脚
    uint16_t led_pin;
    
    // 根据LED编号选择引脚
    if(LEDx == 1)
        led_pin = GPIO_Pin_1;
    else if(LEDx == 2)
        led_pin = GPIO_Pin_2;
    else
        return;  // 无效的LED编号
    
    // 根据状态控制LED
    if(status == 1)  // 开
        GPIO_ResetBits(GPIOA, led_pin);  // 假设低电平点亮
    else if(status == 0)  // 关
        GPIO_SetBits(GPIOA, led_pin);    // 假设高电平熄灭
}

// 使用宏定义使代码更易读
#define LED1    1
#define LED2    2
#define ON      1
#define OFF     0

// 使用示例：
// LEDx_ON(LED1, ON);   // 打开LED1
// LEDx_ON(LED2, OFF);  // 关闭LED2

