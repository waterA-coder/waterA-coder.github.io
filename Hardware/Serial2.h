#ifndef __SERIAL2_H__
#define __SERIAL2_H__

#include <stdio.h>

void Serial2_Init(void);
void Serial2_SendByte(uint8_t Byte);
void Serial2_SendArray(uint8_t *Array, uint16_t Length);
void Serial2_SendString(char *String);
uint32_t Serial2_Pow(uint32_t X, uint32_t Y);
void Serial2_SendNumber(uint32_t Number, uint8_t Length);
int fputc2(int ch, FILE *f);
void Serial2_Printf(char *format, ...);


#endif
