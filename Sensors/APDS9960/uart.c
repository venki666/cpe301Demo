/*
 * USART_RS232_C_file.c
 * http://www.electronicwings.com
 *
 */ 

#include "uart.h"						/* Include USART header file */

void USART_Init(unsigned long BAUDRATE)				/* USART initialize function */
{ 
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);				/* Enable USART transmitter and receiver */
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);	/* Write USCRC for 8 bit data and 1 stop bit */ 
	UBRR0L = BAUD_PRESCALE;							/* Load UBRRL with lower 8 bit of prescale value */
	UBRR0H = (BAUD_PRESCALE >> 8);					/* Load UBRRH with upper 8 bit of prescale value */
}

char USART_RxChar()									/* Data receiving function */
{
	while (!(UCSR0A & (1 << RXC0)));					/* Wait until new data receive */
	return(UDR0);									/* Get and return received data */ 
}

void USART_TxChar(char data)						/* Data transmitting function */
{
	UDR0 = data;										/* Write data to be transmitting in UDR */
	while (!(UCSR0A & (1<<UDRE0)));					/* Wait until data transmit and buffer get empty */
}

void USART_SendString(char *str)					/* Send string of USART data function */ 
{
	int i=0;																	
	while (str[i]!=0)
	{
		USART_TxChar(str[i]);						/* Send each char of string till the NULL */
		i++;
	}
}