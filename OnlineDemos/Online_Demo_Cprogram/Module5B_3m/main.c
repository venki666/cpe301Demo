/*
 * Module5B_3m.c
 *
 * Created: 2/26/2019 5:40:48 PM
 * Author : VenkatesanMuthukumar
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
/* 	- LED connected to PORTB.5
 * 	- Switch connected to PORTD.2 */
int main (void)
{
	/* set PORTB for output*/
	DDRB |= (1<<2)|(1<<5);
	PORTB |= (1<<2)|(1<<5);
	/* set PORTC.2 for input*/
	//DDRC &= ~(1 << 1);
	//PORTC |= (1 << PINC1); // enable pull-up
	while (1) {
		if (!(PINC & (1<<PINC1)))
		{
			PORTB &= ~(1<<2);
			_delay_ms(1000);
		}
		else
			PORTB |= (1<<2);
	}
	return 0;
}

