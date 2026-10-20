## 这是我的STM32项目一的学习经历


'''c
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

uint8_t KeyNum,TempThreshold=27,LightThreshold=100,TempUnit = 0;
uint8_t CurrentTemp = 25;    // 当前温度（假数据）
uint8_t CurrentHumi = 60;    // 当前湿度（假数据）
uint8_t CurrentLight = 45;   // 当前光照（假数据）

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
	
	PageState_t 	CurrentPage = PAGE_MAIN;					//设定初值
	PageSetting_t CurrentParam = PAGE_SETTING_TEMP;	//设定初值
	
	while(1)
	{		
		KeyNum = Key_GetNum();
		switch(CurrentPage){
			
			//主界面
			case PAGE_MAIN:			OLED_ShowString(1,1,"MAIN   "); OLED_ShowString(1,8,"--;--");
			OLED_ShowString(2,1,"Temp:  'C");	OLED_ShowNum(2,6,CurrentTemp,2);
			OLED_ShowString(3,1,"Humi:   %");	OLED_ShowNum(3,7,CurrentHumi,2);
			OLED_ShowString(4,1,"Light:    L");OLED_ShowNum(4,7,CurrentLight,4);
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
			
			//休眠界面 （看门狗）施工ing
			case PAGE_SLEEP:		OLED_ShowString(1,1,"SLEEP  ");
			if(KeyNum == 1) CurrentPage = PAGE_MAIN; break;		// OLED_Clear();
		}
	}
}
'''
