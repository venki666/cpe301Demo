/*
 * DC Motor control
 */ 

#define F_CPU 8000000UL						/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#include <avr/interrupt.h>
#include <stdio.h>							/* Include std. library file */
#include <util/delay.h>						/* Include Delay header file */

volatile uint8_t Direction = 0; 

void ADC_Init()								/* ADC Initialization function */
{
	DDRC = 0x00;							/* Make ADC port as input */
	ADCSRA = 0x87;							/* Enable ADC, with freq/128  */
	ADMUX = 0x40;							/* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)					/* ADC Read function */
{
	ADMUX = 0x40 | (channel & 0x07);		/* set input channel to read */
	ADCSRA |= (1<<ADSC);					/* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));			/* Wait until end of conversion by polling ADC interrupt flag */
	ADCSRA |= (1<<ADIF);					/* Clear interrupt flag */
	_delay_us(1);							/* Wait a little bit */
	return ADCW;							/* Return ADC word */
}

ISR(INT0_vect)
{
	TCCR0B |= (0<<CS00)|(0<<CS01);/* Set Fast PWM with Fosc/64 Timer0 clock */
	_delay_us(5000);							/* Software de-bouncing control delay */
	TCCR0B |= (1<<CS00)|(1<<CS02);/* Set Fast PWM with Fosc/64 Timer0 clock */

}

int main(void)
{
	DDRD &= ~(1<<PD2);						/* Make INT0 pin as Input */
	PORTD |= (1 << PD2);    // turn On the Pull-up
	DDRD |= (1<<PD6);						/* Make OC0 pin as Output */
	EICRA |= (1 << ISC01);    // set INT0 to trigger to falling edge
	EIMSK |= (1 << INT0);     // Turns on INT0
	sei();									/* Enable Global Interrupt */
	ADC_Init();								/* Initialize ADC */
	TCNT0 = 0;								/* Set timer0 count zero */
	TCCR0A |= (1<<WGM00)|(1<<WGM01)|(1<<COM0A1);
	TCCR0B |= (1<<CS00)|(1<<CS02);/* Set Fast PWM with Fosc/64 Timer0 clock */
	while(1)
	{
		OCR0A = (ADC_Read(0)/4);				/* Read ADC and map it into 0-255 to write in OCR0 register */
	}
}
