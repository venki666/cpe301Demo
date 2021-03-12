/*
 * UART_TX_IRQ.c
 *
 * Created: 3/12/2021 1:48:11 AM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
volatile int outpos0;
volatile char outbuf0[80];
void prstr0(char* buf);
;
int main(void)
{
	cli();

	UBRR0H = 0;
	UBRR0L = 8; // 115200

	UCSR0B = (1 << TXEN0); //Turn on TX
	UCSR0B |= (1 << TXCIE0);   // Enable the USART Tx Complete interrupt
	sei();

	prstr0("Hello\r\n"); //Send test message
	_delay_ms(1000);
	while (1);

}

ISR ( USART0_TX_vect )
{
	if ( outpos0 != 0)                 //check for empty buffer
	{
		if (outbuf0[outpos0] == 0)     //check for null at end of string
		{
			outpos0 = 78;				//max buffer size
		} else UDR0 = outbuf0[outpos0];  //buffer not empty or null char send to usart0
		if (outpos0 > 0 && outpos0 < 78) //index in range
		{
			outpos0++;					//increment index
		}
		else
		{
			
			outpos0 = 0;				//set to zero to indicate empty buffer
		}
	}
}

void prstr0(char* buf)		//prints string to usart0
{
	while (outpos0 > 0)		//buffer not empty so wait, so double buffering could help
	{
		_delay_us(20);
	}
	strcpy((char*)outbuf0,buf); //put string in buffer
	UDR0 = outbuf0[0];			//send 1st character into usart0
	outpos0 = 1;				//index is non-zero, the ISR will print the rest.
}