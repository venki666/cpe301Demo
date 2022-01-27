/*
 * TIMER0_N_IRQ.c
 *
 * Created: 2/27/2019 11:32:33 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL
#include<avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
int main(void)
{
	DDRB |= (1 << DDB1); //PB1 as output
	TIMSK0 |= (1 << TOIE0);
	TCNT0 = 56; // initial value
	sei();
	//enable interrupts
	TCCR0B |= (1 << CS01) | (1 << CS00);
	// set prescaler to 64 and start the timer
	while (1)
	{
		//main loop
	}
}
ISR (TIMER0_OVF_vect) // timer0 overflow interrupt
{
	TCNT0 = 56;
	PORTB ^= (1 << DDB1);
}

/*
f_out = 16M/(2*64*200) = 625 hz
*/