/*
 * UART_RX_TX.c
 *
 * Created: 10/21/2019 11:56:16 AM
 * Author : VenkatesanMuthukumar
 */ 



#include <avr/io.h>
#include <stdio.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

void uart_init(void);

/* http://www.ermicro.com/blog/?p=325 */

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

/* http://www.cs.mun.ca/~rod/Winter2007/4723/notes/serial/serial.html */

void uart_init(void) {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	#if USE_2X
	UCSR0A |= _BV(U2X0);
	#else
	UCSR0A &= ~(_BV(U2X0));
	#endif

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void uart_putchar(char c, FILE *stream) {
	if (c == '\n') {
		uart_putchar('\r', stream);
	}
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

char uart_getchar(FILE *stream) {
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

int main(void) {

	uart_init();
	stdout = &uart_output;
	stdin  = &uart_input;
	
	char input;

	while(1) {
		printf("Print the help menu here\n");
		input = getchar();
		printf("You wrote %c\n", input);
		switch (input) {
			case 'S':
				printf("Do things for %c\n", input);
			break;
			case 'M':
				printf("Do things for %c\n", input);
			break;
			default:
			printf("Command not recognized\r\n");
			break;
		}
	}

	return 0;
}
