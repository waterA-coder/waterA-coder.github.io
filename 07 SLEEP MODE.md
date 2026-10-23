# 我的 STM32 OLED 项目开发记录

## 项目简介
这是一个基于 STM32F10x 芯片的开发项目，主要实现了 OLED 屏幕显示和按键交互功能。包含主界面、参数设置和休眠三个状态。

补充：现在在主界面按下KEY2进入休眠模式，省电。

## 核心代码
以下是项目的核心控制代码：

```c
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "Serial.h"
#include "Serial2.h"
#include "dht11.h"
#include "Timer.h"
#include "AD.h"

extern volatile uint8_t DHT_Flag;
volatile uint8_t DHT_Flag = 0; 

uint16_t AD1;
uint8_t tH, tL, hH, hL;
uint8_t KeyNum,TempThreshold=27,LightThreshold=100,TempUnit = 0;
uint8_t CurrentTemp;    // 当前温度
uint8_t CurrentHumi;    // 当前湿度
uint8_t CurrentLight;   // 当前光照
float Voltage1;

typedef enum{
	PAGE_MAIN,		//主界面
	PAGE_SETTING,	//参数界面
	PAGE_SLEEP		//休眠界面
}PageState_t;

typedef enum{
	PAGE_SETTING_TEMP,	//温度阈值
	PAGE_SETTING_LIGHT,	//光照阈值
	PAGE_SETTING_UNIT,	//温度单位
}PageSetting_t;


int main(void)
{
	
	OLED_Init();
	Key_Init();
	Serial_Init();
	Serial2_Init();
	Timer_Init();
	AD_Init();
	EXTI0_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	Delay_ms(1000);
	
	PageState_t 	CurrentPage = PAGE_MAIN;					//设定初值
	PageSetting_t CurrentParam = PAGE_SETTING_TEMP;	//设定初值
	
//测试蓝牙TX功能（正常）	Serial2_SendString("HC-05 OK\r\n");
	
	if (DHT11_Init() == 0) {
    Serial_SendString("DHT11 OK\r\n");
	} else {
    Serial_SendString("DHT11 FAIL\r\n");
	}
	
////测试循环	（成功）
//	while(1) {
//    if (DHT11_Read_Data(&tH, &tL, &hH, &hL) == 0) {
//        Serial_SendString("Temp: "); Serial_SendNumber(tH, 2);
//        Serial_SendString(" Humi: "); Serial_SendNumber(hH, 2);
//        Serial_SendString("\r\n");
//    } else {
//        Serial_SendString("Read FAIL\r\n");
//    }
//    Delay_ms(1000);   // 两次读取至少间隔 1 秒
//	}
	
	while(1)
	{		
		KeyNum = Key_GetNum();
		AD1 = AD_GetValue(ADC_Channel_1);
		int light_percent = (3756 - (int)AD1) * 100 / 3700;
		if (light_percent > 100) 	light_percent = 100;
		if (light_percent < 0) 		light_percent = 0;
		CurrentLight = light_percent;
		if(DHT_Flag == 1){
			DHT_Flag = 0;
				if(DHT11_Read_Data(&tH, &tL, &hH, &hL) == 0){
					CurrentTemp = tH;
					CurrentHumi = hH;
					Serial2_SendString ("T:");Serial2_SendNumber (CurrentTemp,2);Serial2_SendString ("'C,");
					Serial2_SendString ("H:");Serial2_SendNumber (CurrentHumi,2);Serial2_SendString ("%,");					
					Serial2_SendString ("L:");Serial2_SendNumber (CurrentLight,3);Serial2_SendString ("%\r\n");					
					}
			}

		switch(CurrentPage){
			
			//主界面
			case PAGE_MAIN:			OLED_ShowString(1,1,"MAIN   "); OLED_ShowString(1,8,"--;--");
			OLED_ShowString(2,1,"Temp:  'C ");	OLED_ShowNum(2,6,CurrentTemp,2);
			OLED_ShowString(3,1,"Humi:   %   ");	OLED_ShowNum(3,7,CurrentHumi,2);
			OLED_ShowString(4,1,"Light:    % ");OLED_ShowNum(4,7,CurrentLight,3);
			if(KeyNum == 1) CurrentPage = PAGE_SETTING;
			if(KeyNum == 2) CurrentPage = PAGE_SLEEP; break;
			
			//参数界面
			case PAGE_SETTING:	OLED_ShowString(1,1,"SETTING");
			if(TempUnit == 0){ 	OLED_ShowString(2,2,"Temp:  'C"); 	OLED_ShowNum(2,7,TempThreshold,2);}
									else {	OLED_ShowString(2,2,"Temp:  'F"); OLED_ShowNum(2,7,TempThreshold,2);}	//这里单位变化的时候数字没有转化！！
			OLED_ShowString(3,2,"Light:    L"); 	OLED_ShowNum(3,8,LightThreshold,4);
			if(TempUnit == 0) OLED_ShowString(4,2,"TempUnit: C"); 
									else	OLED_ShowString(4,2,"TempUnit: F"); 
			// >选择图标
			OLED_ShowChar(2,1,(CurrentParam == 0) ? '>' : ' ');		//三目运算符
			OLED_ShowChar(3,1,(CurrentParam == 1) ? '>' : ' ');
			OLED_ShowChar(4,1,(CurrentParam == 2) ? '>' : ' ');
			
			switch (CurrentParam){
				case 0:	if(KeyNum == 3){
									if(TempThreshold < 50)	TempThreshold++;
								}
								if(KeyNum == 4){
									if(TempThreshold > 0 )	TempThreshold--; 
								}break;
				case 1:	if(KeyNum == 3){ 
									if(LightThreshold < 100)	LightThreshold++;
								}
								if(KeyNum == 4){
									if(LightThreshold > 0)		LightThreshold--;
								}break;
				case 2:	if(KeyNum == 3)	TempUnit= !TempUnit;
								if(KeyNum == 4)	TempUnit= !TempUnit;
								break; 
			}
			
			if(KeyNum == 1) CurrentPage = PAGE_MAIN;
			if(KeyNum == 2)	CurrentParam = (PageSetting_t)((CurrentParam + 1) % 3);
			break;
			
			//休眠界面
			case PAGE_SLEEP:		OLED_ShowString(1,1,"SLEEP  ");
			OLED_WriteCommand(0xAE);
			TIM_Cmd(TIM2,DISABLE);
			PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
			SystemClock_Recover();    // 把 72MHz 找回来
			OLED_Init();              // 重新点亮屏幕
			Timer_Init();             // 重新启动定时器
			 while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0); 
 			// 直接切回主界面
			CurrentPage = PAGE_MAIN;  break;		// OLED_Clear();
		}
	}
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		DHT_Flag = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
```
