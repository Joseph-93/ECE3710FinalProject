#include "LCD.h"
#include "stm32l476xx.h"
#include <string.h>
#include "uart.h"
#include "ADC.h"
#include <stdio.h>
#include "DHT22.h"
#include <math.h>




void SPI_Write(uint8_t *txBuffer, int size) 
{ 
	unsigned char dummyrx[8];
	int i = 0; 
	for (i = 0; i < size; i++) { 
		//Wait for TXE (Transmit buffer empty) 
		while( (SPI3->SR & SPI_SR_TXE ) != SPI_SR_TXE ); 
		SPI3->DR = txBuffer[i]; 
//	 Wait for RXNE (Receive buffer not empty) 
		while( (SPI3->SR & SPI_SR_RXNE ) != SPI_SR_RXNE ); 
		dummyrx[i] = SPI3->DR; 
			// Wait for BSY flag cleared 
	}
	while( (SPI3->SR & SPI_SR_BSY) == SPI_SR_BSY ); 
}

void SPI_Read(uint8_t *rxBuffer, int size) 
{
	int i = 0; 
	for (i = 0; i < size; i++) { 
		// Wait for TXE (Transmit buffer empty) 
		while( (SPI3->SR & SPI_SR_TXE ) != SPI_SR_TXE ); 
		// The clock is controlled by master. 
		// Thus, the master must send a byte 
		SPI3->DR = 0xFF; // A dummy byte 
		// data to the slave to start the clock. 
		while( (SPI3->SR & SPI_SR_RXNE ) != SPI_SR_RXNE ); 
		rxBuffer[i] = SPI3->DR;
	}
	// Wait for BSY flag cleared 
	while( (SPI3->SR & SPI_SR_BSY) == SPI_SR_BSY ); 
}

void SPI_Init()
{ 
	//pc10, 11, 12 af
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN; // Enable SPI3 Clock 
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI3RST; // Reset SPI3 
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_SPI3RST; 
	
	SPI3->CR1 &= ~SPI_CRl_SPE; //disable
	SPI3->CR1 &= ~SPI CRl RXONLY; //full duplex
	SPI3->CR1 &= ~SPI_CR1_BIDIMODE; // bidirectional
	SPI3->CR1 &= ~SPI_CRl_BIDIOE; //output disabled
	//data frame format
	SPI3->CR2 &= -SPI_CR2_DS;
	SPI3->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_l | SPI_CR2_DS_2; // 0111: 8-bit 
	//bit order
	SPI3->CRl &= -SPI_CRl_LSBFIRST; 
	//clock phase
	SPI3->CRl &= -SPI_CRl_CPHA; 
	//clock polarity
	SPI3->CRl &= -SPI_CRl_CPOL; 
	//baud rate control
	// SPI baud rate is set to 5 MHz 
	SPI3->CRl I= 3U<<3; 
	//crc polynomial
	SPI3->CRCPR = 10; 
	//disable hardware crc
	SPI3->CRl &= -SPI_CRl_CRCEN; 
	//frame format
	SPI3->CR2 &= -SPI_CR2_FRF; 
	//software slave management
	SPI3->CRl I= SPI_CRl_SSM;
	//set as master
	SPI3->CRl I= SPI_CRl_MSTR; 
	//nss managed by software
	SPI3->CRl I= SPI_CRl_SSI; 
	//nss pulse management
	SPI3->CR2 I= SPI_CR2_NSSP; 
	//rxne
	SPI3->CR2 I= SPI_CR2_FRXTH; 
	//spi enable
	SPI3->CRl I= SPI_CRl_SPE;
}

void read_Pressure(uint8_t* pdata)
{
	//press msb 0xf7
	//press lsb 0xf8
	//press xlsb 0xf9
	//lower csb
	GPIOC->ODR &= ;
	//write read register for pressutre
	SPI_write((unsigned char)0xF7), 8);
	//read 3 bytes
	SPI_Read(pdata, 24);
	//pull up csb
	GPIOC->
}
