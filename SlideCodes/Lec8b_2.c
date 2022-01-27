/*
 * sprintf_example.c
 *
 * Created: 10/10/2019 9:18:52 PM
 * Author : VenkatesanMuthukumar
 */ 

#define BAUD 9600
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void USART_send(char data); // Used to send integer to terminal
void USART_putstring(char* StringPtr); // Used to take in every character in the string and sends it to the terminal
void USART_init(void); // Initializes the analog to digital functions, as well as OVF interrupt

void USART_init( void )
{
	UBRR0H = 0;
	UBRR0L = F_CPU/16/BAUD - 1; // Used for the BAUD prescaler
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void USART_send(char data)
{
	while (!(UCSR0A & (1 << UDRE0))); // Until UDRE0 goes high, it will keep looping
	UDR0 = data; // UDR0 register grabs the value given from the parameter
	
}

void USART_putstring(char *StringPtr)
{
	while ((*StringPtr != '\0')){ // Until it reaches the end of the line, it will keep looping
		while (!(UCSR0A & (1 << UDRE0))); // Until UDRE0 goes high, it will keep looping
		UDR0 = *StringPtr; // UDR0 register grabs the value given from the parameter
		StringPtr++; // but it does it by every character as shown here
	}
}

int main(void)
{
	char out_str[30] = {0};     // string to print to and transmit
	float flt_num = 31.8394;    // float number to print to string

	USART_init();

	while (1) {
			sprintf(out_str, "flt_num = %f\r\n", flt_num);
			USART_putstring(out_str);
			flt_num += 0.0020;
	}
}