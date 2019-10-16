/*
 * USART_RS232_C_file.c
 * http://www.electronicwings.com
 *
 */ 

#include "USART_RS232_H_file.h"						/* Include USART header file */

void USART_Init(unsigned long BAUDRATE)				/* USART initialize function */
{ 
	UCSRB |= (1 << RXEN) | (1 << TXEN);				/* Enable USART transmitter and receiver */
	UCSRC |= (1 << URSEL)| (1 << UCSZ0) | (1 << UCSZ1);	/* Write USCRC for 8 bit data and 1 stop bit */ 
	UBRRL = BAUD_PRESCALE;							/* Load UBRRL with lower 8 bit of prescale value */
	UBRRH = (BAUD_PRESCALE >> 8);					/* Load UBRRH with upper 8 bit of prescale value */
}

char USART_RxChar()									/* Data receiving function */
{
	while (!(UCSRA & (1 << RXC)));					/* Wait until new data receive */
	return(UDR);									/* Get and return received data */ 
}

void USART_TxChar(char data)						/* Data transmitting function */
{
	UDR = data;										/* Write data to be transmitting in UDR */
	while (!(UCSRA & (1<<UDRE)));					/* Wait until data transmit and buffer get empty */
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