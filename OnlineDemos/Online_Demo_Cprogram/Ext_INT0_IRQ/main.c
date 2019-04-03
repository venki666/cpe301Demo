/*
 * Ext_INT0_IRQ.c
 *
 * Created: 2/26/2019 7:49:16 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

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
	}
}

ISR (INT0_vect)//ISR for external interrupt 0
{
	PORTB ^= (1<<5);//toggle PORTB.5
}
