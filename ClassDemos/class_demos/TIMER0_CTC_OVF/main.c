/*
 * TIMER0_CTC_OVF.c
 *
 * Created: 2/27/2019 10:37:51 PM
 * Author : VenkatesanMuthukumar
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
int main(void)
{
	DDRD |= (1 << DDD6); // PD6 is an output
	DDRB |= (1 << DDB1); //PB1 as output
	// Set the Timer Mode to CTC and PD6 toggle mode
	TCCR0A |= (1 << WGM01); // | (0 << COM1A0);
	// Set the value that you want to count to
	OCR0A = 0xF9; //249
	// initialize counter
	TCNT0 = 0;
	// set prescaler to 64 and start the timer
	TCCR0B |= (1 << CS01) | (1 << CS00);
	while (1)
	{
	    while ((TIFR0 & 0x02) == 0);
	    PORTB ^= (1 << DDB1);
		TCNT0=0x00;
		TIFR0=0x02; // reset the overflow flag
	}
}
/*
f_out = 16M/(2*64*250) = 500 hz
*/
