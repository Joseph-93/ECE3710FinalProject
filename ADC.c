#include "ADC.h"

void ADC1_setup(){
	GPIOC->ASCR |= 0x00000030;//set bit in ascr to close analog switch
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	ADC1->CR &= ~ADC_CR_ADEN; //disable adc 
	
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	ADC123_COMMON->CCR |= 0x00010000;
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
	if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD) {
		ADC1->CR &= ~ADC_CR_DEEPPWD; //exit power-down mode if still in that state
	}
	ADC1->CR |= ADC_CR_ADVREGEN;
	int wait_time = 80 * (80000000/1000000);
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

uint16_t read_ADC1(){
	//Read ADC
	uint16_t result = 0;
	ADC1->CR |= 0x00000004; //Start adc conversion
	while ((ADC1->ISR & ADC_ISR_EOS) == 0); //if the end of conversion is flagged
	result = ADC1->DR & 0x0000FFFF; // set result to the found value
	//ADC1->CR &= ~ADC_CR_ADSTART; //Turn off ADSTART just in case
	return result;
}

float internal_temperature_c(uint16_t raw)
{
	float TS_CAL1 = 0x0407;
	float TS_CAL2 = 0x0513;
	float T;
	T = (110.0-30.0)/(TS_CAL2-TS_CAL1)*((float)raw*1.1-TS_CAL1)+30.0;
	return T;
	
}


void WindSpeed_setup()
{
	//enable clock to correct pin
	GPIOC->MODER |= 0x00000F00;//set pin mode to analog
	GPIOC->ASCR |= 0x00000030;//set bit in ascr to close analog switch
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	ADC2->CR &= ~ADC_CR_ADEN; //disable acd 2
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN; //enable voltage boost
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN; //enable vref
	ADC123_COMMON->CCR |= 0x00010000;  //clock mode
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL; // seting to single mode (independent) ???
	if ((ADC2->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD) {
		ADC2->CR &= ~ADC_CR_DEEPPWD; //exit power-down mode if still in that state on adc 2
	}
	//set resolution to 12 bits (deflault)
	//set right align (default)
	ADC2->CR |= ADC_CR_ADVREGEN; //enable advreg
	ADC2->SQR1 &= ~ADC_SQR1_L; // Sequence length to 1
	ADC2->SQR1 &= ~ADC_SQR1_SQ1; // Specify the channel number of the first conversion in regular sequence (clears sequence register)
	ADC2->SQR1 |= (13U << 6); //Set to pin c4
	int wait_time = 80 * (80000000/1000000);
	while(wait_time != 0)
	{
		wait_time--;
	}
	//calibrate adc
	ADC2->CR |= ADC_CR_ADCAL; //calibrate adc 2
	while((ADC2->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL); //wait for calibration to finish

	
	
	
	ADC2->SMPR1 |= ADC_SMPR1_SMP4_2; //Set sampling time to ..long
	ADC2->SMPR1 |= ADC_SMPR1_SMP5_2; //Set sampling time to ..long
	ADC2->CFGR &= ~ADC_CFGR_CONT; //set to discontintuous
	ADC2->CFGR &= ~ADC_CFGR_EXTEN; //set to code trigger
	ADC2->CR |= ADC_CR_ADEN; //Re-enable ADC1
	while((ADC2->ISR & ADC_ISR_ADRDY) == 0);
}
	
uint32_t read_Tref(){
	//Read ADC
	uint32_t result = 0;
	ADC2->CR |= 0x00000004; //Start adc conversion
	while ((ADC2->ISR & ADC_ISR_EOS) == 0); //if the end of conversion is flagged
	result = ADC2->DR; // set result to the found value
	//ADC1->CR &= ~ADC_CR_ADSTART; //Turn off ADSTART just in case
	//while ((ADC2->ISR & ADC_ISR_EOS) == 0);
	//wait til end of conversion sequence (throw away extra vals)
	return result;
}
float temp_convert(uint32_t in)
{
	float temp;
	temp = (in-0.76)*72;
	return(temp);
}
