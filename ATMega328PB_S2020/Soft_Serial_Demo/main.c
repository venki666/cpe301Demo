/*
 * Soft_Serial_Demo.c
 *
 * Created: 8/4/2020 3:50:10 AM
 * Author : venkim
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "SWseriale.h" // SWseriale library
// SWseriale example
// Standard settings:
// RX -> Pin 3
// TX -> Pin 4

int main(void)
{
	sei();
	SWseriale_begin(); // Initialize INT1, Timer2, Pin 3 (Input, Pull-up) and Pin 4 (Output)
	
	SWseriale_write("HI",2);
	
	/* Replace with your application code */
	while (1)
	{
		while (SWseriale_available()){ // Checks if any character has been received
			uint8_t temp = SWseriale_read(); // Reads one character from SWseriale received data buffer
			SWseriale_write(&temp, 1); // Send one character using SWseriale
		}

		_delay_ms(10); // Wait 10 ms, optional
	}
}