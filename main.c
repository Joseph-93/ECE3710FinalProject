#include "stm32l476xx.h"
#include "LCD.h"
#include <string.h>
#include "uart.h"
#include "ADC.h"
#include "DHT22.h"

static char temp[50];
static char humidity[50];
static char pressure[50];

//void set_globals()
//{
//	temp = 0;
//	humidity = 0;
//	pressure = 0;
//}

//Yellow Button | PC0 | Temperature

void EXTI0_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF0) != 0) {
		//read value
		int rh_raw = 0;
		float rh_dec = 0.0f;
		int t_raw = 0;
		float t_dec = 0.0f;
		uint8_t data[41];
		uint8_t out[40];
		for(int i = 1; i <41; i++)
		{
			data[i]='q';
		}
		read_hdata(data);
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
		//display value
		LCD_Clear();
		LCD_DisplayString(0, (unsigned char*)"Temperature: (C)");
		LCD_Displayfloat(1, t_dec);
		EXTI->PR1 |= EXTI_PR1_PIF0;
	}
	return;
}

//Blue Button | PC1 | Humidity
void EXTI1_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF1) != 0) {
		//read value
		//read value
		int rh_raw = 0;
		float rh_dec = 0.0f;
		int t_raw = 0;
		float t_dec = 0.0f;
		uint8_t data[41];
		uint8_t out[40];
		for(int i = 1; i <41; i++)
		{
			data[i]='q';
		}
		read_hdata(data);
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
		//display value
		LCD_Clear();
		LCD_DisplayString(0, (unsigned char*)"Humidity: (%)");
		LCD_Displayfloat(1, rh_dec);
		EXTI->PR1 |= EXTI_PR1_PIF1;
	}
	return;
}

//Green Button | PC2 | Wind Speed
void EXTI2_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF2) != 0) {
		//read value
		int w_raw = 0;
		float w_v = 0;
		LCD_Clear();
		LCD_DisplayString(0, (unsigned char*)"Wind Speed:");
		w_raw = read_Wind();
		EXTI->PR1 |= EXTI_PR1_PIF2;
		w_v = (pow(10.0f,((float)w_raw/1120.0f))/76.0f)-1.0f;
		LCD_Displayfloat(1, (float)w_v);
	}
	return;
}

//Red Button | PC3 | Air Pressure
void EXTI3_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF3) != 0) {
		//read value
		LCD_Clear();
		LCD_DisplayString(0, (unsigned char*)"Air Pressure: X");
		EXTI->PR1 |= EXTI_PR1_PIF3;
	}
	return;
}

int main(void){
	
	//clock setup
	RCC->CR |= 0x00000100;//enable hsi clock
	while(RCC->CR != 0x00000563)
	{
		//wait for hsi ready flag
	}
	RCC->CFGR |= 0x00000001; //switch system clock to hsi
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; //enable clock for gpio c
	GPIOC->MODER &= 0XFFFFFF00; //set gpioc registers 0-3 to input mode
	GPIOC->PUPDR |= 0X00000055; //set gpioc registers to be internal pull up resistors
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
	//set up interrupts for each button
	
		//set up EXTI
	
	//enable interrupt
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	
	//connect EXTI to GPIO ports
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC;
	
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PC;
	
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PC;
	
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PC;
	
	//Enable falling trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT0;
	EXTI->FTSR1 |= EXTI_FTSR1_FT1;
	EXTI->FTSR1 |= EXTI_FTSR1_FT2;
	EXTI->FTSR1 |= EXTI_FTSR1_FT3;
	
	//Interrupt Mask Register
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	EXTI->IMR1 |= EXTI_IMR1_IM1;
	EXTI->IMR1 |= EXTI_IMR1_IM2;
	EXTI->IMR1 |= EXTI_IMR1_IM3;
	
	LCD_Init();
	usart2_init();
	USART2_Stringout((unsigned char*)"Weather station: \n");
	USART2_Stringout((unsigned char*)"Press T for temperature H for humidity W for windspeed and P for Pressure");
	LCD_Clear();
	LCD_DisplayString(0, (unsigned char*)"Weather Station");
	LCD_DisplayString(1, (unsigned char*)"USU ECE 3700");
	WindSpeed_setup();
	ADC1_setup();
	DHT22_setup();
	
	//TIM6 setup for dht22 reading
	TIM6->CR1 &= 0xFE00; //Clear out necessary bits
	TIM6->CR1 |= 0x0A07; //set necessary bits
	TIM6->CR2 &= 0xFF8F; //RESET TIMER
	TIM6->CNT |= 0x0000; //Set counter to 0
	TIM6->PSC &= 0x0000; //set prescaler to none
	TIM6->ARR |= 0xFFFF; //set auto-reload to MAX
	
	GPIOC->MODER &= 0xFFFFDFFF;
	
	while(1)
	{
//		delay_us(360000);
//		uint8_t data[40];
//		read_hdata(data);
	}
}
