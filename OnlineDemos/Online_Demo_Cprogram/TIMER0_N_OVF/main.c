/*
 * TIMER0_N_OVF.c
 *
 * Created: 2/27/2019 11:17:18 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL

#include<avr/io.h>
#include <util/delay.h>
int main(void)
{
	DDRB |= (1 << DDB1); //PB1 as output
	TCCR0A = 0; // Normal Operation
	TCNT0=0x06;
	// start the timer
	TCCR0B |= (1 << CS01) | (1 << CS00);
	// set prescaler to 64 and start the timer
	while (1)
	{
		// wait for the overflow event
		while ((TIFR0 & 0x01) == 0);
		PORTB ^= (1 << DDB1);
		TCNT0=0x06;
		TIFR0=0x01; // reset the overflow flag
	}
}
/*
f_out = 16M/(2*64*256) = 488.281 hz
*/