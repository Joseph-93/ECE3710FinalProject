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
	GPIOC->MODER |= 0x0005000;//set pc6 to output &7
	GPIOC->ODR |= 0x00000040;//pull up pc6
}
uint8_t read_hdata(uint8_t* data)
{
	int in;
	int count = 0;
	int in_high = 0x00000040;
	
	GPIOC->ODR &= 0xFFFFFFBF;//pull down output
	delay_us(40000);//delay 1 ms
	GPIOC->ODR |= 0x00000040;//pull it back up
	//delay_us(53); //delay 30 us
	GPIOC->MODER &= 0xFFFFCFFF; //clear pc6 mode (set to input)
	in = (GPIOC->IDR & 1<<6); //read_ADC1 gpio 6 input
//	delay_us(280);//wait 160 us
	while(in == 0) //wait during first low (80 us)
	{
		in = (GPIOC->IDR & 1<<6);
	}
	while(in == in_high) //wait during first high (80 us) 
	{
		in = (GPIOC->IDR & 1<<6);
	}
	
	for(int i = 0; i<41;i++)//repeat 40 times
	{
		while(in == 0) //wait during first low 
		{
			in = (GPIOC->IDR & 1<<6);
		}
		count = 0;
		while(in == 0)//wait for start of data (50us)
		{
			in = (GPIOC->IDR & 1<<6);
		}
		TIM6->CR2 &= 0xFF8F;
		TIM6->CR2 |= 0x0010;
		while(in == in_high) //count length of pulse
		{
			in = (GPIOC->IDR & 1<<6);
			count++;
		}
		data[i]=(uint8_t)count;
		//while sensor low
				//count time high
				//if time hih < 50 0
				//if time high > 50 1
////		if((TIM6->CNT & 0x0000FFFF) <= 700)
////		{
////			data[i] = count;
////		}
////		else
////		{
////			data[i] = count;
////		}
	}
	DHT22_setup();
	
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