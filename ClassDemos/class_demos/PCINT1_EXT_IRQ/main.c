/*
 * PCINT1_EXT_IRQ.c
 *
 * Created: 2/26/2019 11:05:37 PM
 * Author : VenkatesanMuthukumar
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
int main(void)
{
	DDRB |= 1<<5; // Set PortB.5 as output
	PORTB |= (1<<5); // Clearing PortB.5
	DDRC &= (0<<1)|(0<<2)|(0<<3); // Setting PinC 1/2/3 as inputs
	PORTC |= (1<<1)|(1<<2)|(1<<3); // Turning on the pullups
	// Enable PC at pins 9/10/11
	PCMSK1 |= (1 << PCINT9)|(1 << PCINT10)|(1 << PCINT11);
	PCICR |= (1<<PCIE1); // Enable PC Set 1 interrupt
	sei(); // Enable Global interrupt

	while(1);
}

ISR(PCINT1_vect)
{
	PORTB ^= (1<<5);
	//_delay_ms(1000);
}

