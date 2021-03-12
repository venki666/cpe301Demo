/*
 * TIMER1_CTC_ICR_Waveform.c
 *
 * Created: 3/12/2021 12:24:13 AM
 * Author : VenkatesanMuthukumar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


int main(void)
{
	PORTD &= ~((1<<5) | (1<<6));
	//set PD6 and PD5 as output; but only PD5 used!?
	DDRD |= (1<<5) | (1<<6);
	TCNT1 = 0;
	ICR1 = 4096;
	OCR1A = 666;
	OCR1B = 1333;
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	// Mode 12, CTC on ICR1
	TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B) | (1 << ICIE1);
	//Set interrupt on compare match
	TCCR1B |= (1 << CS12);
	// set prescaler to 256 and starts the timer
	sei();
	PORTD |= (1<<5) | (1<<6);
	// enable interrupts
	while (1)
	{
		// we have a working Timer
	}
}

ISR (TIMER1_COMPA_vect)
{
 PORTD &= ~(1<<5);
}

ISR (TIMER1_COMPB_vect)
{
 PORTD &= ~(1<<6);
}

ISR (TIMER1_CAPT_vect)
{
 PORTD |= (1<<5) | (1<<6);
 TCNT1 = 0;
}

