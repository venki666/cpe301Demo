
/*
 * The program reads the internal temperature of the MCU using ADC and sends it to the PC.
 * If you put your finger on the MCU, the number increases.
 *
 * Created: 05/11/2017 02:05:47
 *  Author: Naimi
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600

#include <avr/io.h>		
#include <util/delay.h>
#include <stdlib.h>
void usart_init ();
void usart_send (unsigned char ch);
void USART_putstring(char* StringPtr);
char buffer[5];
int main (void)
{
	usart_init ();
	
	ADCSRA= 0x87;			//make ADC enable and select ck/128
	ADMUX= 0xC8;			//1.1V Vref, temp, right-justified, internal temp. sensor
	
	while (1)
	{
		ADCSRA|=(1<<ADSC);	//start conversion
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		ADCSRA |= (1<<ADIF);
		int a = ADCL;
		a = a | (ADCH<<8);
		a -= 289;
		itoa(a, buffer, 10); 
		USART_putstring(buffer); 
		USART_putstring("\n"); 
		_delay_ms(100);
	}
	return 0;
}


void usart_init (void)
{
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
}

void usart_send (unsigned char ch)
{
	while (! (UCSR0A & (1<<UDRE0))); 	//wait until UDR0 is empty
	UDR0 = ch;							//transmit ch
}

void USART_putstring(char* StringPtr) {

	while (*StringPtr != 0x00) {
		usart_send(*StringPtr);
		StringPtr++;
	}

}