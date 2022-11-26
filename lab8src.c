//bit 23 of adc_ccr should be 1

/*usart congfiguration
usart_cr1 : bit 28 00, bit 15 oversampling, bit 10 0,bit 3 1, bit 2 1,bit 0 enable
brr: 1a1

*/
#include "stm32l476xx.h"
#define N 1848
#include <stdio.h>

void delay_ms(unsigned int ms) {
	unsigned int volatile i = 0;
	while(i<(N*ms))
	{
		i++;
	}	
}

void usart2_init()
{
	USART2->CR1 &= ~USART_CR1_UE;
	USART2->CR1 &= ~USART_CR1_M;
	USART2->CR2 &= ~USART_CR2_STOP;
	USART2->CR1 &= ~USART_CR1_PCE;
	USART2->CR1 &= ~USART_CR1_OVER8;
	USART2->BRR = 0x1A1;
	USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	USART2->CR1 |= USART_CR1_UE;
	while ((USART2->ISR & USART_ISR_TEACK) == 0);
	while ((USART2->ISR & USART_ISR_REACK) == 0);
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

void ADC_setup(){
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	ADC1->CR &= ~ADC_CR_ADEN;
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	ADC123_COMMON->CCR |= 0x00010000;
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
	if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD) {
		ADC1->CR &= ~ADC_CR_DEEPPWD; //exit power-down mode if still in that state
	}
	ADC1->CR |= ADC_CR_ADVREGEN;
	int wait_time = 20 * (80000000/1000000);
	while(wait_time != 0)
	{
		wait_time--;
	}
	//calibrate adc
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL);
	//ADC1->CFGR &= 0X0000000;
	
	//ADC1->CR |= ADC_CR_ADVREGEN;
	//delay_ms(1); //The ADC requires a wait time in the rance of microseconds, so we will just wait 1ms
	ADC123_COMMON->CCR |= 0x00800000; // Set CCR to channel select 17
	ADC1->SQR1 &= ~ADC_SQR1_L; // Sequence length to 1
	ADC1->SQR1 &= ~ADC_SQR1_SQ1; // Specify the channel number of the first conversion in regular sequence 17  
	ADC1->SQR1 |= (17U << 6); //Set to ADC12_IN17 << change to 17
	ADC1->SMPR2 |= ADC_SMPR2_SMP17_2; //Set sampling time to ..long
	ADC1->CFGR &= ~ADC_CFGR_CONT; //set to discontintuous
	ADC1->CFGR &= ~ADC_CFGR_EXTEN; //set to code trigger
	ADC1->CR |= ADC_CR_ADEN; //Re-enable ADC1
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
	
}

uint16_t read_ADC(){
	//Read ADC
	uint16_t result = 0;
	ADC1->CR |= 0x00000004; //Start adc conversion
	while ((ADC1->ISR & ADC_ISR_EOS) == 0); //if the end of conversion is flagged
	result = ADC1->DR & 0x0000FFFF; // set result to the found value
	//ADC1->CR &= ~ADC_CR_ADSTART; //Turn off ADSTART just in case
	return result;
}

float temperature_c(uint16_t raw)
{
	float TS_CAL1 = 0x0418;
	float TS_CAL2 = 0x051E;
	float T;
	T = (110.0-30.0)/(TS_CAL2-TS_CAL1)*((float)raw*1.1-TS_CAL1)+30.0;
	return T;
	
}

int main(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //enable port a
	
	GPIOA->MODER = 0xABFFFFAF;
	GPIOA->OSPEEDR = 0x0C0000F0;
	GPIOA->AFR[0] = 0x00007700;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	GPIOA->ASCR = 0x00020000;
	
	usart2_init();
	uint16_t in =0;
	
	USART2_Write('s');
	USART2_Write('t');
	USART2_Write('a');
	USART2_Write('r');
	USART2_Write('t');
	
	ADC_setup();
	
	float result = 0.0;
//	
//	USART2_Write(result);


	while(1)
	{
		in = USART2_Read();
		float temp;
		char out[50];
		//USART2_Write((uint8_t)in);
		//USART2_Write('\n');
		if(in == 'T' || in == 't')
		{
			result = read_ADC();
			USART2_Write('t');
			USART2_Write('e');
			USART2_Write('m');
			USART2_Write('p');
			USART2_Write(':');
			USART2_Write(' ');
			temp = temperature_c(result);
			sprintf(out, "%f" , temp);
			for(int i= 0; i<50;i++)
			{
				USART2_Write(out[i]);
			}
	
		}
		//USART2_Write(result);
		USART2_Write('\n');
	}
}