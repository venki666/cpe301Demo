/*
 * USART_RS232_H_file.h
 * http://www.electronicwings.com
 *
 */ 

#define F_CPU 16000000UL					/* Define CPU clock Frequency e.g. here its 12MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#include <math.h>

#define DOUBLE_SPEED_MODE

#ifdef DOUBLE_SPEED_MODE
	#define BAUD_PRESCALE (int)round(((((double)F_CPU / ((double)BAUDRATE * 8.0))) - 1.0))	/* Define prescale value */
#else
	#define BAUD_PRESCALE (int)round(((((double)F_CPU / ((double)BAUDRATE * 16.0))) - 1.0))	/* Define prescale value */
#endif

void USART_Init(unsigned long);				/* USART initialize function */
char USART_RxChar();						/* Data receiving function */
void USART_TxChar(char);					/* Data transmitting function */
void USART_SendString(char*);				/* Send string of USART data function */


