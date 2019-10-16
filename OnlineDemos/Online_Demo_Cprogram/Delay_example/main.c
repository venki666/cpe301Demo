/*
 * Delay_example.c
 *
 * Created: 8/21/2019 11:34:18 AM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU	16000000UL	// set CPU speed
#include <avr/io.h>			// include io.h
#include <util/delay.h>		// include delay.h

int main(void)				// program entry
{
	DDRB = 0XFF;			// set PORTB to output
	PORTB = 0X3B;			// clear PINS connect to LEDs
	while (1)				// loop forever
	{
		PORTB &= ~(1<<2);	// clear PORTB.2
		_delay_ms(500);		// delay 500ms
		PORTB |= (1<<2);	// set PORTB.2
		_delay_ms(500);		// delay 500ms
	}
}
