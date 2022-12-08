#ifndef UART_H
#define UART_H
#include "stm32l476xx.h"
#include "LCD.h"
#include <string.h>

void usart2_init(void);
uint16_t USART2_Read(void);
void USART2_Write(uint8_t data);
void USART2_IRQHandler(void);
void USART2_Stringout(char* out);
void USART2_fout(float data);
#endif

