/*
 * ADC_example.c
 *
 * Created: 10/10/2019 10:14:02 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void usart_init ();
void adc_init();
void timer_init();

void usart_send (unsigned char ch);

int main (void)
{
	timer_init ();
	usart_init ();
	adc_init ();
	
	while (1)
	{
		/*
		ADCSRA|=(1<<ADSC);	//start conversion
		while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
		
		ADCSRA |= (1<<ADIF);

		int a = ADCL;
		a = a | (ADCH<<8);
		a = (a/1024.0) * 5000/10;
		usart_send((a/100)+'0');
		a = a % 100;
		usart_send((a/10)+'0');
		a = a % 10;
		//a = a * (9/5) + 32; // celsius to fahrenheit
		usart_send((a)+'0');
		usart_send('\r');
		
		_delay_ms(100);
		*/
	}
	return 0;
}

ISR (TIMER1_OVF_vect)
{
	ADCSRA|=(1<<ADSC);	//start conversion
	while((ADCSRA&(1<<ADIF))==0);//wait for conversion to finish
	
	ADCSRA |= (1<<ADIF);
	
	int a = ADCL;
	a = a | (ADCH<<8);
	a = (a/1024.0) * 5000/10;
	usart_send((a/100)+'0');
	a = a % 100;
	usart_send((a/10)+'0');
	a = a % 10;
	//a = a * (9/5) + 32; // celsius to fahrenheit
	usart_send((a)+'0');
	usart_send('\r');
	_delay_ms(100);
	TCNT1 = 49911; // Reset timer
}

void usart_init (void)
{
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = F_CPU/16/BAUD_RATE-1;
}

void adc_init (void)
{
	/** Setup and enable ADC **/
	ADMUX = (0<<REFS1)|    // Reference Selection Bits
	(1<<REFS0)|    // AVcc - external cap at AREF
	(0<<ADLAR)|    // ADC Left Adjust Result
	(1<<MUX2)|     // Analog Channel Selection Bits
	(0<<MUX1)|     // ADC4 (PC4 PIN27)
	(1<<MUX0);
	ADCSRA = (1<<ADEN)|    // ADC ENable
	(0<<ADSC)|     // ADC Start Conversion
	(0<<ADATE)|    // ADC Auto Trigger Enable
	(0<<ADIF)|     // ADC Interrupt Flag
	(0<<ADIE)|     // ADC Interrupt Enable
	(1<<ADPS2)|    // ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
}

void timer_init (void)
{
	TCCR1B |= 5; //(1 << CS12) | (1 << CS10); // Sets prescaler to 1024
	TIMSK1 = (1 << TOIE1); // Enables overflow flag
	TCNT1 = 49911; // 1 second delay = (0xFFFF) - TCNT = 65535 - 15624 = 49911
	sei();
}

void usart_send (unsigned char ch)
{
	while (! (UCSR0A & (1<<UDRE0))); 	//wait until UDR0 is empty
	UDR0 = ch;							//transmit ch
}

void usart_print(char* str)
{
	int i = 0;
	while(str[i] != 0)
	usart_send(str[i]);
}

