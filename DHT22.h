#ifndef DHT22_H
#define DHT22_H
#include "stm32l476xx.h"
#include "DHT22.h"
#include <string.h>
void DHT22_setup(void);
uint8_t read_hdata(uint8_t*);
void delay_us(int us);
//put functions here
#endif