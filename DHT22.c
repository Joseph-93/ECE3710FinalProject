#include "stm32l476xx.h"
#include "LCD.h"
#include <string.h>
#include "uart.h"
#include "ADC.h"
#include <stdio.h>
#define UN 2

void delay_us(int us)
{
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

uint8_t read_data(uint8_t* data)
{
	int in;
	GPIOC->MODER &= 0xFFFFCFFF; //clear pc6 mode
	GPIOC->MODER |= 0x0001000;//set pc6 to output
	GPIOC->PUPDR |= 0x00001000;// add pullup resistor
	GPIOC->ODR &= 0xFFFFFFDF;//pull down output for 20-40us?
	delay_us(48);//delay 30 us(should it be 24?)
	GPIOC->MODER &= 0xFFFFCFFF; //clear pc6 mode (set to input)
	delay_us(121);//wait for 75 us
	in = (GPIOC->IDR & 1<<6);
	if(in == 1)//if pc6 == 1
	{
		return(0);// 	sensor fail
							//		return 0
	}
	else //else
	{
		//	proceed reading data 
	  for(int i = 0; i < 40; i++)//	for (i = 0, i <40, i++)
		{
	//			wait 65 us
	//			if pc6 high
	//				data[i] = 1
	//			else
//					data[i]= 0
		}//	endfor
	//return(1)
	
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