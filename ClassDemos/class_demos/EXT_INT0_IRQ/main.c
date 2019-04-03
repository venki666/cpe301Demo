/*
 * EXT_INT0_IRQ.c
 *
 * Created: 2/26/2019 10:57:32 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main ()
{
	DDRB |= (1<<5) | (1<<4);//PB5 as an output
	PORTD |= 1<<2;//pull-up activated
	EICRA = 0x2;//make INT0 falling edge triggered

	EIMSK = (1<<INT0);//enable external interrupt 0
	sei ();//enable interrupts

	while (1)
	{
		PORTB ^= (1<<4);	//wait here
		_delay_ms(1000);
	}
}

ISR (INT0_vect)//ISR for external interrupt 0
{
	PORTB ^= (1<<5);//toggle PORTB.5
	_delay_ms(1000);
}
