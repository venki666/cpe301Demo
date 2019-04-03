/*
 * Timer1_N_Longest.c
 *
 * Created: 3/7/2019 9:13:35 PM
 * Author : VenkatesanMuthukumar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

// TIMER1 overflow interrupt service routine
// called whenever TCNT1 overflows
ISR(TIMER1_OVF_vect)
{
	PORTB ^= (1 << DDB2);  // toggles the led
}

int main(void)
{
	// connect led to pin PB2
	DDRB |= (1 << DDB2);
	// initialize timer
	// set up timer with pre-scalar = 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);
	// initialize counter
	TCNT1 = 0;
	// enable overflow interrupt
	TIMSK1 |= (1 << TOIE1);
	// enable global interrupts
	sei();
	// loop forever
	while(1)
	{
		// do nothing
	}
}