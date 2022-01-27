/*
 * Stepper Motor Control
 *
 */ 


#define F_CPU 16000000UL				/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>					/* Include AVR std. library file */
#include <util/delay.h>				/* Include delay header file */


int main(void)
{
	int period;
	DDRD = 0x0F;					/* Make PORTD lower pins as output */
	period = 9000;					/* Set period in between two steps of Stepper Motor */
	// setup ADC
	//setup Timer 0 in CTC mode
	// CMP0A = 0;
	while (1)
	{
		// read_ADC();
		// period = SF * ADC_value;
		// CMP0A = period;
		for(int i=0;i<500;i++)		/* Rotate Stepper Motor clockwise with Half step sequence; Half step angle 3.75 */
		{
			PORTD = 0x09;
			_delay_us(period);
			PORTD = 0x08;
			_delay_us(period);
			PORTD = 0x0C;
			_delay_us(period);
			PORTD = 0x04;
			_delay_us(period);
			PORTD = 0x06;
			_delay_us(period);
			PORTD = 0x02;
			_delay_us(period);
			PORTD = 0x03;
			_delay_us(period);
			PORTD = 0x01;
			_delay_us(period);
		}
			PORTD = 0x09;			/* last one step to acquire initial position */ 
			_delay_us(period);
			_delay_ms(1000);
		//for(int i=0;i<500;i++)		/* Rotate Stepper Motor Anticlockwise with Full step sequence; Full step angle 7.5 */
		//{
			//PORTD = 0x09;
			//_delay_us(period);
			//PORTD = 0x03;
			//_delay_us(period);
			//PORTD = 0x06;
			//_delay_us(period);
			//PORTD = 0x0C;
			//_delay_us(period);
		//}
			//PORTD = 0x09;
			//_delay_us(period);
			//_delay_ms(1000);
	}
}