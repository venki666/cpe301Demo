/*
 * ICM20948_SPI.c
 *
 * Created: 4/26/2021 11:47:09 PM
 * Author : VenkatesanMuthukumar
 */ 


#define F_CPU 16000000UL			/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>					/* Include AVR std. library file */
#include <util/delay.h>				/* Include delay header file */
#include <inttypes.h>				/* Include integer type header file */
#include <stdlib.h>					/* Include standard library file */
#include <stdio.h>					/* Include standard library file */
#include <avr/interrupt.h>			/* Include avr interrupt header file */
#include "uart.h"
volatile uint8_t dataX, dataY; 
uint16_t raw, temperature;
#define DEVICE_ID_ICM20948				0xEA
void spi_init(void)
{
	//SS, MOSI, SCK
	DDRB |= (1 << 2) | (1 << 3) | (1 << 5);
	//MISO
	DDRB &= ~(1 << 4);
	
	SPCR0 = (0 << SPIE) |
	(1 << SPE)  |
	(0 << DORD) |
	(1 << MSTR) |
	(0 << CPOL) |
	(0 << CPHA) |
	(0 << SPR1) | (1 << SPR0);
}

	
void spi_send( uint8_t send_data )
{
	SPDR0 = send_data;
	//loop_until_bit_is_set(SPSR0, SPIF);
	while(!(SPSR0 & (1<<SPIF)));
	//SPSR0 |= (1 << SPIF);
}

//exchange of single 8-bit value from sensor
uint8_t spi_transrecv( uint8_t send_data )
{
	SPDR0 = send_data;
	//loop_until_bit_is_set(SPSR0, SPIF);
	while(!(SPSR0 & (1<<SPIF)));
	return SPDR0;
}

int main(void)
{
	char buffer[30], float_[5];
	USART_Init(9600);	
	USART_SendString("UART Initialized\n");
	spi_init();
	USART_SendString("SPI Initialized\n");
	PORTB &= ~(1<<2);
	spi_send(0x80);
	dataX=spi_transrecv(0x00);
	PORTB |= (1<<2);
	if(dataX != DEVICE_ID_ICM20948)
	 USART_SendString("Device Not Found\n");
	else
	 USART_SendString("Device Found\n");
	
	return 0;
}

