/*
 * TIMER0_CTCC_IRQ.c
 *
 * Created: 2/27/2019 11:01:53 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
int main(void)
{
	DDRD |= (1 << DDD6); // PD6 is an output
	OCR2A = 62; //Load Compare Reg value
	TCCR2A |= (1 << WGM21); // Set to CTC Mode
	TIMSK2 |= (1 << OCIE2A); //Set interrupt on compare match
	TCCR2B |= (1 << CS22); // set prescaler to 64 and starts PWM
	sei(); // enable interrupts
	while (1)
	{ // Main loop
	}
}
ISR (TIMER2_COMPA_vect)
{
	PORTD ^= (1 << DDD6); // can be any output
}

/*
f_out = 16M/(2*64*63) = 1984.126984126984 hz
*/
