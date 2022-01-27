/*
 * GPIO_example.c
 *
 * Created: 2/23/2021 10:30:06 AM
 * Author : VenkatesanMuthukumar
 */ 


#define F_CPU 16000000UL // Arduino UNO use a 16Mhz Crystal as Clock Source

#include <avr/io.h> // Contains all the I/O Register Macros
#include <util/delay.h> // Generates a Blocking Delay

int main(void)
{
	DDRB |= 1 << 5; // Configuring PB5 as Output
	DDRC &= ~(1<<1); // Configuring PC1 as Input
	PORTC |= 1<<1; // Configuring PC1 Pull-Up at Input
	while (1)
	{
		if ((PINC & (1<<1))) // Reading the Pin Value PC1
		{
			PORTB |= 1<<5; // Writing HIGH to glow LED
		}
		else
		{
			PORTB &= ~(1<<5); // Writing LOW
		}
	}
}
