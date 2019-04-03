/*
 * PCINT_IRQ.c
 *
 * Created: 2/26/2019 8:14:26 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRB |= 1<<5;
	PORTB |= (1<<5);
	DDRC &= (0<<1)|(0<<2)|(0<<3);
	PORTC |= (1<<1)|(1<<2)|(1<<3);
	PCMSK1 |= (1 << PCINT9)|(1 << PCINT10)|(1 << PCINT11);
	PCICR |= (1<<PCIE1);
	sei();

	while(1);
}

ISR(PCINT1_vect)
{
	PORTB ^= (1<<5);
}
