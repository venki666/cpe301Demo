//	Set clock frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//	Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "inc\STDIO_UART.c"

//	Include nRF24L01+ library
#include "inc\nrf24l01.c"
#include "inc\nrf24l01-mnemonics.h"
#include "inc\spi.c"
void print_config(void);

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

void ADC_init (void);
volatile unsigned char ADCtemp[5];
volatile uint8_t ADCvalue;

int main(void)
{
	//	Set cliche message to send (message cannot exceed 32 characters)
	char tx_message[32];				// Define string array
	strcpy(tx_message,"It's working!");	// Copy string into array
	
	//	Initialize UART
	uart_init();
	
	//  Initialize ADC
	ADC_init();
	
	//	Initialize nRF24L01+ and print configuration info
	nrf24_init();
	print_config();
	
	//	Start listening to incoming messages
	nrf24_start_listening();
	
	strcpy(tx_message,"Initializing Chat Room...");	// Copy string into array
	nrf24_send_message(tx_message);
	
	while (1)
	{		
		if (message_received)
		{
			//	Message received, print it
			message_received = false;
			printf("Received Temperature: %s\n\n",nrf24_read_message());
			
			//	Send message as response
			_delay_ms(500);
			status = nrf24_send_message(ADCtemp);
			if (status == true) printf("Temperature Successfully Sent\n\n");
		}
	}
}

//	Interrupt on IRQ pin
ISR(INT0_vect)
{
	message_received = true;
}

//  Interrupt for Temperature Sensor
ISR(ADC_vect)
{
	volatile unsigned int j=0;
	char temp[5];
	
	ADCvalue = (ADCH << 1);		 // Shifts the value left to one place
	
	itoa(ADCvalue, temp, 10);	 // Converts integers to string
	// Takes ADCvalue, turns it into an ASCII representation
	// the ASCII representation will be stored under 'temp'
	// '10' represents the buffer
	
	while (j<5)					 // Transfers the temp string from itoa() to ADCtemp
	{
		ADCtemp[j] = temp[j];
		j++;
	}
}

//  Prints configuration
void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}

void ADC_init (void)
{
	//ADC Multiplexer Selection Register
	ADMUX = (1 << REFS0) |		 // Voltage reference during conversion, "AVcc with external capacitor at AREF pin"
	(1 << ADLAR);		 // Left adjust ADC conversion result in ADC Data Register
	
	//ADC Control and Status Register A
	ADCSRA = (1 << ADEN) |		 // ADC enable
	(1 << ADSC) |		 // ADC Start Conversion
	(1 << ADATE) |		 // ADC Auto Trigger enable
	(1 << ADIE) |		 // ADC Interrupt enable
	(1 << ADPS2) |
	(1 << ADPS1) |
	(1 << ADPS0);		 // ADPS2:0 = 111 = 128 prescaler
}