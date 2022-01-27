/*
 * ATmega16_Servo_Motor.c
 * http://www.electronicwings.com
 */ 

#define F_CPU 8000000UL								/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>									/* Include AVR std. library file */
#include <stdio.h>									/* Include std. library file */
#include <util/delay.h>								/* Include Delay header file */


void ADC_Init()										/* ADC Initialization function */
{
	DDRA=0x00;								        /* Make ADC port as input */
	ADCSRA = 0x87;									/* Enable ADC, with freq/128  */
	ADMUX = 0x40;									/* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)							/* ADC Read function */
{
	ADMUX = 0x40 | (channel & 0x07);				/* set input channel to read */
	ADCSRA |= (1<<ADSC);							/* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));					/* Wait until end of conversion by polling ADC interrupt flag */
	ADCSRA |= (1<<ADIF);							/* Clear interrupt flag */
	_delay_ms(1);									/* Wait a little bit */
	return ADCW;									/* Return ADC word */
}

int main(void)
{
	ADC_Init();										/* Initialize ADC */
	DDRD |= (1<<PD5);								/* Make OC1A pin as output */ 
	TCNT1 = 0;										/* Set timer1 count zero */
	ICR1 = 2499;									/* Set TOP count for timer1 in ICR1 register */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);				/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	while(1)
	{
		OCR1A = 65 + (ADC_Read(0)/4.35);			/* Read ADC channel 0 and make count in between 65-300 i.e. 0.5-2.4 mSec duty cycle variation */
	}
}
