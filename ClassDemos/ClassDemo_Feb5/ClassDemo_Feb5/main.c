/*
 * ClassDemo_Feb5.c
 *
 * Created: 2/5/2019 11:34:02 AM
 * Author : VenkatesanMuthukumar
 */ 


#define F_CPU 1600000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void){
	DDRC|= (1<<4)|(1<<5); // set PC4 and PC5 as outputs
	PORTC = (0 << 4) | (1 << 5);
	while(1){
		PORTC ^= (1<<4); // toggle the pin
		PORTC ^= (1<<5); // toggle the pin
		_delay_us(10);
	}
}


