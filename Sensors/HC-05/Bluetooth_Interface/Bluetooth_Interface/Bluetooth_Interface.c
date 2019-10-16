/*
   Bluetooth_Interface with ATmega16 to Control LED via smartphone
   http://www.electronicwings.com
 
 */ 

#include <avr/io.h>
#include "USART_RS232_H_file.h"		/* include USART library */

#define LED PORTB					/* connected LED on PORT pin */

int main(void)
{
	char Data_in;
    DDRB = 0xff;							/* make PORT as output port */
	USART_Init(9600);						/* initialize USART with 9600 baud rate */
	LED = 0;
	
	while(1)
	{
		Data_in = USART_RxChar();						/* receive data from Bluetooth device*/
		if(Data_in =='1')
		{
			LED |= (1<<PB0);							/* Turn ON LED */
			USART_SendString("LED_ON");					/* send status of LED i.e. LED ON */
			
		}
		else if(Data_in =='2')
		{
			LED &= ~(1<<PB0);							/* Turn OFF LED */
			USART_SendString("LED_OFF"); 				/* send status of LED i.e. LED OFF */
		}
		else
			USART_SendString("Select proper option");	/* send message for selecting proper option */
	}		
	return 0;
}