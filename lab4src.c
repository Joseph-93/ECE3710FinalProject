#include "stm32l476xx.h"
#include "LCD.h"



int main(void){
	
	//clock setup
	RCC->CR |= 0x00000100;//enable hsi clock
	while(RCC->CR != 0x00000563)
	{
		//wait for hsi ready flag
	}
	RCC->CFGR |= 0x00000001; //switch system clock to hsi
	//enable clock for gpio b
	
	LCD_Init();
	LCD_Clear();
	LCD_DisplayString(0, (unsigned char*)"ECE 3710");
	LCD_DisplayString(1, (unsigned char*)"Utah State");
	
	//test delay
	//RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //enable clock for gpio b
	//GPIOA->MODER &= 0xABFFF7FF;//set gpiob to output
	//while(1)
	//{
	//	GPIOA->ODR |= 0x00000020;
	//	delay_ms(1000);
	//	GPIOA->ODR &= 0xFFFFFFDF;
	//	delay_ms(1000);
	//}
	
	
	while(1);
}


