//Define MCU CPU Freq for the time delay functions
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "ds18b20.h"
#include "uart.h"

int main(void) {
	char printbuff[100];
	double d = 0;
	//init uart
	USART_Init( 9600);
	//init interrupt
	sei();
	for (;;) {
		d = ds18b20_gettemp();
		dtostrf(d, 10, 3, printbuff);
		USART_SendString("Temperature: "); 
		USART_SendString(printbuff); 
		USART_SendString("\r\n");
		_delay_ms(500);
	}
	return 0;
}
