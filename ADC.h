#ifndef ADC_H
#define ADC_H
#include "stm32l476xx.h"
#include "LCD.h"
#include <string.h>

void ADC1_setup(void);
uint16_t read_ADC1(void);
float internal_temperature_c(uint16_t raw);
void WindSpeed_setup(void);
uint32_t read_Tref(void);
float temp_convert(uint32_t);
#endif