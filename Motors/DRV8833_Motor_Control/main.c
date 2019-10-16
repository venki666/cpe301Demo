/*
 * DRV8833_Motor_Control.c
 *
 * Created: 8/26/2019 4:53:27 PM
 * Author : VenkatesanMuthukumar
 */ 

#include <avr/io.h>


int main(void)
{
	DDRD |= (1 << DDD6)|(1 <<DDD5);
	// PD6 is now an output

	OCR0A = 128;
	OCR0B = 60;
	// set PWM for 50% duty cycle


	TCCR0A |= (1 << COM0B1); //| (1<<COM0B1)  
	// set none-inverting mode

	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// set fast PWM Mode

	TCCR0B |= (1 << CS02) | (1 << CS00);
	// set prescaler to 1024 and starts PWM


	while (1);
	{
		// we have a working Fast PWM
	}
}