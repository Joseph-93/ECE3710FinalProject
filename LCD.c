#include "LCD.h"
#include "stm32l476xx.h"
#include <string.h>
#include "uart.h"
#include "ADC.h"
#include <stdio.h>
#include "DHT22.h"
#include <math.h>
#define N 1800

/********************
function: delay_ms
input: milisecond value (unsigned int)
output:none

********************/
void delay_ms(unsigned int ms) {
	unsigned int volatile i = 0;
	while(i<(N*ms))
	{
		i++;
	}	
}

/********************
function: LCD_WriteCom
input: command to send
output: none
Begin
	Clear ODR
	seperate data
	send data (1)
	enable
	wait
	clear register
	wait
	send data (3)
	enable
	wait
	clear register
	de-enable
	wait

	wait
********************/
void LCD_WriteCom(unsigned char com) {
	GPIOB->ODR &= 0x00000000; //clear odr
	unsigned char com1 = 0;
	unsigned char com2 = 0;
	com1 = com & 0xF0;
	com2 = com & 0x0F;
	com1 = com1 >> 4;
	GPIOB->ODR |= com1;
	GPIOB->ODR |= 0x40;
	delay_ms(1);
	GPIOB->ODR &= 0x0F;
	delay_ms(1);
	GPIOB->ODR &= 0xF0;
	GPIOB->ODR |= com2;
	GPIOB->ODR |= 0x40;
	delay_ms(1);
	GPIOB->ODR &= 0x0F;
	//GPIOB->ODR &= 0x00;
	
}

/********************
function: LCD_WriteData
input: charachter to wrie (char)
output: none
begin
	set rs high
	set rw low
	
	

********************/
void LCD_WriteData(unsigned char dat) {
	GPIOB->ODR &= 0x00000000; //clear odr
	unsigned char dat1 = 0;
	unsigned char dat2 = 0;
	dat1 = dat & 0xF0;
	dat2 = dat & 0x0F;
	dat1 = dat1 >> 4;
	GPIOB->ODR |= 0x10;
	GPIOB->ODR |= dat1;
	GPIOB->ODR |= 0x40;
	delay_ms(1);
	GPIOB->ODR &= 0xBF;
	delay_ms(1);
	GPIOB->ODR &= 0xF0;	
	GPIOB->ODR |= dat2;
	GPIOB->ODR |= 0x40;
	delay_ms(1);
	GPIOB->ODR &= 0xBF;
	//GPIOB->ODR &= 0x00;
}

/********************
function: LCD_Init
input: none
output: none
This function sets up the GPIO and LCD for use by the other functions

Begin
	Enable GPIO b clock
	configure gpio b as output
	write com (0x2)
	delay 1ms
	write com (0x2)
********************/
void LCD_Init(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //enable clock for gpio b
	GPIOB->MODER &= 0X00000000; // Reset gpiob to 0's
	GPIOB->MODER |= 0x55555555;//set gpiob to output
	//
	LCD_WriteCom(0x28);
	delay_ms(1);
	LCD_WriteCom(0x28);
	delay_ms(1);
	LCD_WriteCom(0x0C);
	delay_ms(1);
	LCD_WriteCom(0x06);
	delay_ms(1);
	LCD_WriteCom(0x01);
	delay_ms(1);
	LCD_WriteCom(0x14);
	delay_ms(1);
	LCD_WriteCom(0x40);
	delay_ms(1);
}

/********************
function: LCD_Clear
input: none
output: none

********************/
void LCD_Clear(void){
	LCD_WriteCom(0x01);
	delay_ms(1);
	LCD_WriteCom(0x80);
	delay_ms(1);
	
}

/********************
function: LCD_DisplayString
input: line number (int), string (pointer to string)
output: none

********************/
void LCD_DisplayString(unsigned int line, unsigned char *ptr) {
	int n = 0;
	unsigned int location = 0x80;
	location = location + 0x40*(line);
	LCD_WriteCom((char)location);
	
	while(ptr[n] != 0)
	{
		LCD_WriteData(ptr[n]);
		n++;
	}
}

void LCD_Displayfloat(unsigned int line, float in)
{
	unsigned char out[50];
	for(int i = 0; i <=50; i++)
		{
			out[i]=0;
		}
	sprintf(out, "%f" , in);
	LCD_DisplayString(line, out);
}
