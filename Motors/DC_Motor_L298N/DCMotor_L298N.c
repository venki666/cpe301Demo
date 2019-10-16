/*
 * DC_Motor_L298N.c
 *
 * Created: 8/26/2019 4:10:09 PM
 * Author : VenkatesanMuthukumar
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL // 16MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD |= 0xF0; //PORTD as Output
	while(1)
	{
		//Rotates Motor in Antilockwise
		PORTD = 0x40; 
		_delay_ms(4000);

		//Stops Motor
		PORTD = 0x00; 
		_delay_ms(4000);

		//Rotates Motor in Clockwise
		PORTD = 0x80; 
		_delay_ms(4000);

		//Stops Motor
		PORTD = 0xC0; //00000011
		_delay_ms(4000);
	}
}