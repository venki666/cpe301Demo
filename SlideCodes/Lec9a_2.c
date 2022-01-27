/*
 * ADC2_example.c
 *
 * Created: 10/10/2019 10:24:06 PM
 * Author : VenkatesanMuthukumar
 */ 

#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

uint16_t adc_value;            //Variable used to store the value read from the ADC
char buffer[5];                //Output of the itoa function
uint8_t i=0;                //Variable for the for() loop

void adc_init(void);            //Function to initialize/configure the ADC
uint16_t read_adc(uint8_t channel);    //Function to read an arbitrary analogic channel/pin
void USART_init(void);            //Function to initialize and configure the USART/serial
void USART_send( unsigned char data);    //Function that sends a char over the serial port
void USART_putstring(char* StringPtr);    //Function that sends a string over the serial port

int main(void){
	adc_init();        //Setup the ADC
	USART_init();        //Setup the USART
	
	for(;;){        //Our infinite loop
	/*	for(i=0; i<3; i++){
			//USART_putstring("Reading channel ");
			//USART_send('0');            //This is a nifty trick when we only want to send a number between 0 and 9
			//USART_putstring(" : ");            //Just to keep things pretty
			adc_value = read_adc(i);        //Read one ADC channel
			itoa(adc_value, buffer, 10);        //Convert the read value to an ascii string
			USART_putstring(buffer);        //Send the converted value to the terminal
			USART_putstring(",");            //Some more formatting
			_delay_ms(500);                //You can tweak this value to have slower or faster readings or for max speed remove this line
		*/
		adc_value = read_adc(4);        //Read one ADC channel
		itoa(adc_value, buffer, 10);        //Convert the read value to an ascii string
		USART_putstring(buffer);        //Send the converted value to the terminal
		USART_putstring(",");            //Some more formatting
		adc_value = read_adc(5);        //Read one ADC channel
		itoa(adc_value, buffer, 10);        //Convert the read value to an ascii string
		USART_putstring(buffer);        //Send the converted value to the terminal
		USART_send('\n');             //Some more formatting
		//}
		//USART_send('\r');
		//USART_send('\n');                //This two lines are to tell to the terminal to change line
	*/
	
	}
	
	return 0;
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

void USART_init(void){
	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}
	
}