#include "stm32l476xx.h"
#include "LCD.h"
#include <string.h>
#include "uart.h"
#include "ADC.h"

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
		LCD_Clear();
		LCD_DisplayString(0, (unsigned char*)"Temperature: ");
		EXTI->PR1 |= EXTI_PR1_PIF0;
	}
	return;
}

//Blue Button | PC1 | Humidity
void EXTI1_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF1) != 0) {
		//read value
		LCD_Clear();
		LCD_DisplayString(0, (unsigned char*)"Humidity: X");
		EXTI->PR1 |= EXTI_PR1_PIF1;
	}
	return;
}

//Green Button | PC2 | Wind Speed
void EXTI2_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF2) != 0) {
		//read value
		LCD_Clear();
		LCD_DisplayString(0, (unsigned char*)"Wind Speed: X");
		EXTI->PR1 |= EXTI_PR1_PIF2;
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
	USART2_Write('s');
	USART2_Write('t');
	USART2_Write('a');
	USART2_Write('r');
	USART2_Write('t');
	USART2_Stringout((unsigned char*)"ECE 3710\n");
	USART2_Stringout((unsigned char*)"Utah State\n");
	LCD_Clear();
	LCD_DisplayString(0, (unsigned char*)"ECE 3710");
	LCD_DisplayString(1, (unsigned char*)"Utah State");
	WindSpeed_setup();
	ADC1_setup();
	

	
	while(1)
	{
//		USART2_fout((float)(read_Tref()));
//		delay_ms(2000);
	}
}
