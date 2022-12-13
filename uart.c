#include "stm32l476xx.h"
#include "LCD.h"
#include <string.h>
#include "uart.h"
#include "ADC.h"
#include <stdio.h>
#include "DHT22.h"
#include <math.h>
#define BufferSize 32

static uint8_t USART2_Buffer_Rx[BufferSize];
static uint32_t Rx2_Counter;

void usart2_init()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //enable port a
	
	GPIOA->MODER = 0xABFFFFAF;
	GPIOA->OSPEEDR = 0x0C0000F0;
	GPIOA->AFR[0] = 0x00007700;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	
	//Rx2_Counter = 0;
	USART2->CR1 &= ~USART_CR1_UE;
	USART2->CR1 &= ~USART_CR1_M;
	USART2->CR2 &= ~USART_CR2_STOP;
	USART2->CR1 &= ~USART_CR1_PCE;
	USART2->CR1 &= ~USART_CR1_OVER8;
	USART2->BRR = 0x683;
	USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	USART2->CR1 |= USART_CR1_UE;
	while ((USART2->ISR & USART_ISR_TEACK) == 0);
	while ((USART2->ISR & USART_ISR_REACK) == 0);
	
	USART2->CR1 |= USART_CR1_RXNEIE;
	USART2->CR1 &= ~USART_CR1_TXEIE;
	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART2_IRQn);
	
}
uint16_t USART2_Read ()
{
	while((USART2->ISR & USART_ISR_RXNE) == 0);
	return(USART2->RDR);
}
void USART2_Write(uint8_t data)
{
	int i = 0;
	while(!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = data;
	while(!(USART2->ISR & USART_ISR_TC));
	USART2->ICR |= USART_ICR_TCCF;
}

void USART2_IRQHandler(void) {
	int rh_raw = 0;
	float rh_dec = 0.0f;
	int t_raw = 0;
	float t_dec = 0.0f;
	char in = 0;
	if(USART2->ISR & USART_ISR_RXNE) {// Check RXNE event 
		in = USART2->RDR;
	}
	if(in == 'T'|| in == 't')
	{
		//USART2_Write('2');
		uint8_t data[41];
		for(int i = 1; i <41; i++)
		{
			data[i]='q';
		}
		read_hdata(data);
//		for(int i = 1; i<41; i++)
//		{
//			USART2_fout((float)data[i]);
//			USART2_Write(';');
//		}
//		USART2_Write('!');
		uint8_t out[40];
		for (int i=1;i<41;i++)
		{
			if(data[i]<100)
			{
				out[i-1] = 0;
			}
			else
			{
				out[i-1] = 1;
			}
		}
//			for(int i = 0; i<40; i++)
//			{
//				USART2_Write(out[i]+48);
//			}
		USART2_Write(' ');
		//rh part
		for(int i = 0; i<=15;i++)
		{
			rh_raw += out[i]*pow(2,15-i);
		}
		rh_dec = (float)rh_raw/10.0f;
		
		for(int i = 16; i<32;i++)
		{
			t_raw += out[i]*pow(2,15-(i-16));
		}
		t_dec = (float)t_raw/10.0f;
		
		USART2_fout(t_dec);
		USART2_Write(' ');
		USART2_Write('C');
		//USART2_fout(t_dec);
	}
	else if(in == 'H'|| in == 'h')
	{
		//USART2_Write('2');
		uint8_t data[41];
		for(int i = 1; i <41; i++)
		{
			data[i]='q';
		}
		read_hdata(data);
//		for(int i = 1; i<41; i++)
//		{
//			USART2_fout((float)data[i]);
//			USART2_Write(';');
//		}
//		USART2_Write('!');
		uint8_t out[40];
		for (int i=1;i<41;i++)
		{
			if(data[i]<100)
			{
				out[i-1] = 0;
			}
			else
			{
				out[i-1] = 1;
			}
		}
//			for(int i = 0; i<40; i++)
//			{
//				USART2_Write(out[i]+48);
//			}
		USART2_Write(' ');
		//rh part
		for(int i = 0; i<=15;i++)
		{
			rh_raw += out[i]*pow(2,15-i);
		}
		rh_dec = (float)rh_raw/10.0f;
		
		for(int i = 16; i<32;i++)
		{
			t_raw += out[i]*pow(2,15-(i-16));
		}
		t_dec = (float)t_raw/10.0f;
		
		USART2_fout(rh_dec);
		USART2_Write(' ');
		USART2_Stringout("% relative humidity");
		//USART2_fout(t_dec);
	}
	else if(in == 'W'|| in == 'w')
	{
		int w_raw = 0;
		float w_v = 0;
		//USART2_Write('1');
		w_raw = read_Wind();
		USART2_fout((float)(w_raw));
		w_v = (pow(10.0f,((float)w_raw/1120.0f))/76.0f)-1.0f;
		USART2_fout((float)(w_v));
		//read temp value
		//dispaly temp
	}
	else if(in == 'P'|| in == 'p')
	{
		USART2_Write('x');
		//read pressure
		//display pressure
	} 
} 

void USART2_Stringout(char* out)
{
	int i = 0;
	while(out[i] != 0)
	{
		USART2_Write(out[i]);
		i++;
	}
}

void USART2_fout(float data)
{
	char out[50];
	for(int i = 0; i <=50; i++)
		{
			out[i]=0;
		}
	sprintf(out, "%f" , data);
	for(int i= 0; i<50;i++)
	{
		USART2_Write(out[i]);
	}

}


