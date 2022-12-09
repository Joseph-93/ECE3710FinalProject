#include "stm32l476xx.h"
#include "LCD.h"
#include <string.h>
#include "uart.h"
#include "ADC.h"
#include <stdio.h>
#include "DHT22.h"
#define UN 1

void delay_us(int us)
{
	//magic ratio delay (us = us*1.75)
	unsigned int volatile i = 0;
	while(i<(UN*us))
	{
		i++;
	}	
}
	
void DHT22_setup()
{
	GPIOC->MODER &= 0xFFFFCFFF; //clear pc6 mode
	GPIOC->MODER |= 0x0001000;//set pc6 to output
	GPIOC->PUPDR |= 0x00001000;// add pullup resistor
	GPIOC->ODR &= 0xFFFFFFDF;//pull down output for 20-40us?
	delay_us(48);//delay 30 us(should it be 24?)
	GPIOC->MODER &= 0xFFFFCFFF; //clear pc6 mode (set to input)
	//delay for 30 us
}

uint8_t read_hdata(uint8_t* data)
{
	int in;
	int count;
	int in_high = 0x00000040;
	GPIOC->MODER &= 0xFFFFCFFF; //clear pc6 mode
	GPIOC->MODER |= 0x0001000;//set pc6 to output
	GPIOC->PUPDR |= 0x00001000;// add pullup resistor
	GPIOC->ODR &= 0xFFFFFFDF;//pull down output for 20-40us?
	delay_ms(40);//delay 600 us
	GPIOC->MODER &= 0xFFFFCFFF; //clear pc6 mode (set to input)
	in = (GPIOC->IDR & 1<<6);
	while(in == in_high)
	{
		in = (GPIOC->IDR & 1<<6);
	}//wait until pulldown
	while(in == in_high)
	{
		in = (GPIOC->IDR & 1<<6);
	}//wait until sensor pulls up
	while(in == in_high)
	{
		in = (GPIOC->IDR & 1<<6);
	}//wait for sensor to go low (80ms) this is the start of data bit1
	for(int i = 0; i<40;i++)//repeat 40 times
	{
		while(in == in_high)
		{
			in = (GPIOC->IDR & 1<<6);
		}//wait for sensor to go back up (first data bit)
		while(in == in_high)
		{
			in = (GPIOC->IDR & 1<<6);
			count++;
		}//while sensor low
				//count time high
				//if time hih < 50 0
				//if time high > 50 1
		if(count <= 50)
		{
			data[i] = 0;
		}
		else
		{
			data[i] = 1;
		}
	}
	return(1);//return(1)
	
}

/*void start_signal()
 { DataDir = 0;     // Data port is output
   Data    = 0;
   __delay_ms(18);
   DataDir = 1;     // Data port is input
   __delay_us(30);
 }

unsigned short check_response()
 { TOUT = 0;
   TMR2 = 0;
   TMR2ON = 1;                // start timer
   while (!Data && !TOUT);
   if (TOUT) 
      return 0;
   else 
    { TMR2 = 0;
      while (Data && !TOUT);
      if (TOUT) 
         return 0;
      else 
       { TMR2ON = 0;
         return 1;
       }
    }
 }

unsigned short read_byte()
 { unsigned short num = 0;
   DataDir = 1;
   for (i=0; i<8; i++)
    { while (!Data);
      TMR2 = 0;
      TMR2ON = 1;
      while (Data);
      TMR2ON = 0;
      if (TMR2 > 40) 
         num |= 1<<(7-i);  // If time > 40us, Data is 1
    }
   return num;
 }*/