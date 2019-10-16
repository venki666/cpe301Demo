/*
 * SPI_Master_C_file.c
 *
 * http://www.electronicwings.com
 */ 

#include "spi_master.h"

void SPI_SS_Enable()												/* Enable the SS pin to logic zero */		
{
	PORTB &= ~(1<<SS);
}

void SPI_SS_Disable()												/* disable the SS pin to logic one */
{
	PORTB |= (1<<SS);
}

void SPI_Init()
{
	DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS)|(1<<RST)|(1<<DC);				/* Set MOSI and SCK output, all others input */
	PORTB |= (1<<RST);												/* define reset pin as a output */
	DDRB &= ~(1<<MISO);												/* define MISO pin as a input */
	PORTB |= (1<<SS);												/* Make SS pin as a output */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);							/* Enable SPE, MSTR and SPR0 bit in SPI control register */
}

char SPI_Read()
{
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));										/* wait here while SPIF flag set */
	return(SPDR);													/* Return the SPDR value */
}

void SPI_Write(char write_data)										
{
	SPDR = write_data;													
	while(!(SPSR & (1<<SPIF)));										/* wait here while SPIF flag set */
}

