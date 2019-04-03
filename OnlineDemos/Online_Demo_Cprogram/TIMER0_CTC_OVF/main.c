/*
 * TIMER0_CTC_OVF.c
 *
 * Created: 2/27/2019 11:23:15 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
int main(void)
{
	//DDRD |= (1 << DDD6); // PD6 is an output
	DDRB |= (1<<DDB1); //PB1 is an output
	// Set the Timer Mode to CTC and PD6 toggle mode
	TCCR0A |= (1 << WGM01) ; //| (1 << COM1A0);
	// Set the value that you want to count to
	OCR0A = 0xF9; //249
	// initialize counter
	TCNT0 = 0;
	// set prescaler to 64 and start the timer
	TCCR0B |= (1 << CS01) | (1 << CS00);
	while (1)
	{
	 while( (TIFR0 & 0x02) ==0 );
	 PORTB ^= (1 << DDB1);
	 TCNT0 = 0;
	 TIFR0 = 0x02;
	}
}
/*
f_out = 16M/(2*64*250) = 500 hz
*/