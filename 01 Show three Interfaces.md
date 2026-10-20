# 我的 STM32 OLED 项目开发记录

## 项目简介
这是一个基于 STM32F10x 芯片的开发项目，主要实现了 OLED 屏幕显示和按键交互功能。包含主界面、参数设置和休眠三个状态。

## 核心代码
以下是项目的核心控制代码：

```c
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

uint8_t KeyNum;

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
	
	
	PageState_t CurrentPage = PAGE_MAIN;
	PageSetting_t CurrentParam = PAGE_SETTING_TEMP;
	
	while(1)
	{		
		KeyNum = Key_GetNum();
		switch(CurrentPage){
			case PAGE_MAIN:  OLED_ShowString(1,1,"MAIN   "); if(KeyNum == 1) CurrentPage = PAGE_SETTING;if(KeyNum == 2) CurrentPage = PAGE_SLEEP; break;
			case PAGE_SETTING:  OLED_ShowString(1,1,"SETTING");if(KeyNum == 1) CurrentPage = PAGE_MAIN;break;
			case PAGE_SLEEP:  OLED_ShowString(1,1,"SLEEP  ");if(KeyNum == 1) CurrentPage = PAGE_MAIN; break;	//OLED_Clear();
		}
	}
}
```
