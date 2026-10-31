#ifndef __KEY_H_
#define __KEY_H_

void Key_Init(void);
uint8_t Key_GetNum(void);
void EXTI0_IRQHandler(void);
void SystemClock_Recover(void);
void EXTI0_Init(void);


#endif
