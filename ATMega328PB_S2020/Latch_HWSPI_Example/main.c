/*
 * Latch_HWSPI_Example.c
 *
 * Created: 3/19/2022 12:51:10 PM
 * Author : venkim
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif


#include <avr/io.h>
#include <util/delay.h>


#define SHIFT_REGISTER DDRB
#define SHIFT_PORT PORTB
#define DATA (1<<PB3)		//MOSI (SI)
#define LATCH (1<<PB2)		//SS   (RCK)
#define CLOCK (1<<PB5)		//SCK  (SCK)

void init_IO(void){
	//Setup IO
	SHIFT_REGISTER |= (DATA | LATCH | CLOCK);	//Set control pins as outputs
	SHIFT_PORT &= ~(DATA | LATCH | CLOCK);		//Set control pins low
}

void init_SPI(void){
	//Setup SPI
	SPCR0 = (1<<SPE) | (1<<MSTR);	//Start SPI as Master
}

void spi_send(unsigned char byte){
	SPDR0 = byte;			//Shift in some data
	while(!(SPSR0 & (1<<SPIF)));	//Wait for SPI process to finish
}

/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
									0x92, 0x82, 0xF8, 0X80, 0X90};
/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};
	
int main(void)
{
	init_IO();
	init_SPI();
	
	//unsigned int binary_counter = 0;

	while(1)
	{
		//Pull LATCH low (Important: this is necessary to start the SPI transfer!)
		SHIFT_PORT &= ~LATCH;

		//spi_send((unsigned char)(0xA4));
		//spi_send((unsigned char)0xF8);
	/* Show 0-9 pattern for 1 second @ different digits */
	for (uint8_t i = 0; i < 10; i++) {
		spi_send((unsigned char)SEGMENT_MAP[i]);
		spi_send((unsigned char)0xF4);
		//Toggle latch to copy data to the storage register
		SHIFT_PORT |= LATCH;
		SHIFT_PORT &= ~LATCH;
		//wait for a little bit before repeating everything
		_delay_ms(1000);
	}

		//Toggle latch to copy data to the storage register
	//	SHIFT_PORT |= LATCH;
	//	SHIFT_PORT &= ~LATCH;
		//wait for a little bit before repeating everything
	//	_delay_ms(200);
	}
}
