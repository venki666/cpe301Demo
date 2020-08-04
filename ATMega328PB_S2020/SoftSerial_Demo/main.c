/*
 * SoftSerial_Demo.c
 *
 * Created: 8/4/2020 3:50:10 AM
 * Author : venkim
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include <util/atomic.h>
#include "SWserial.h" // SWseriale library
// SWseriale example
// Standard settings:
// RX -> Pin 2
// TX -> Pin 4

uint16_t adc_value;            //Variable used to store the value read from the ADC
char buffer[10];                //Output of the itoa function
uint8_t i=0;                //Variable for the for() loop

void adc_init(void);            //Function to initialize/configure the ADC
uint16_t read_adc(uint8_t channel);    //Function to read an arbitrary analogic channel/pin

int main(void)
{
	adc_init();        //Setup the ADC
	SWseriale_begin(); // Initialize INT1, Timer2, Pin 3 (Input, Pull-up) and Pin 4 (Output)
	sei();
	SWseriale_write("Reading Pot Value:  ",20);
	while(1)
	{
		adc_value = read_adc(0);        //Read one ADC channel
		itoa(adc_value, buffer, 10);        //Convert the read value to an ascii string
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
		  SWseriale_write(buffer,10);        //Send the converted value to the terminal
		  SWseriale_write("\n",1);
		}
		  _delay_ms(500);                //You can tweak this value to have slower or faster readings or for max speed remove this line
	}
}
void adc_init(void){
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (1<<REFS0);                //Voltage reference from Avcc (5v)
	ADCSRA |= (1<<ADEN);                //Turn on ADC
	ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}

uint16_t read_adc(uint8_t channel){
	ADMUX &= 0xF0;                    //Clear the older channel that was read
	ADMUX |= channel;                //Defines the new ADC channel to be read
	ADCSRA |= (1<<ADSC);                //Starts a new conversion
	while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
	return ADCW;                    //Returns the ADC value of the chosen channel
}

