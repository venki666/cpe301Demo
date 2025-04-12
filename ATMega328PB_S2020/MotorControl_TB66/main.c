/*
 * MotorControl_TB66.c
 *
 * Created: 4/19/2024 3:45:42 PM
 * Author : venki
 */ 

/*
 * DC Motor control
 */ 

#define F_CPU 16000000UL						/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#include <avr/interrupt.h>
#include <stdio.h>							/* Include std. library file */
#include <util/delay.h>						/* Include Delay header file */

volatile uint8_t Direction = 0; 

int main(void)
{
  DDRD |=  (1<<DDD6) | (1 << DDD5) | (1 << DDD4); /* Make OC0 pin as Output */
  // We are manually setting the direction
  PORTD |= (1 << DDD4);               // CW Direction Set
  PORTD &= ~(1 << DDD5);              // CW Direction Set
  // WE are not using the ADC for speed - just manually setting the value
  // ADC_Init(); /* Initialize ADC */
  TCNT0 = 0;  /* Set timer0 count zero */
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
  TCCR0B |=  (1 << CS01); /* Set Fast PWM with Fosc/64 Timer0 clock */
  OCR0A = 65;
  while(1)
  {
	  
  }
}