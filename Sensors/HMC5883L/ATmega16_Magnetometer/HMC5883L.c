 /*
 * ATmega16_Magnetometer.c
 * http://www.electronicwings.com
 * 
 */ 

#define F_CPU 16000000UL									/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>										/* Include AVR std. library file */
#include <stdlib.h>										/* Include std. library file */
#include <math.h>										/* Include math header file */
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "i2c_master.h"							/* Include I2C header file */
#define PI	3.14159265359								/* Define Pi value */
#define Declination	-0.00669							/* Define declination of location from where measurement going to be done. e.g. here we have added declination from location Pune city,India. we can get it from http://www.magnetic-declination.com */

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
void USART_init( unsigned int ubrr );
void USART_tx_string( char *data );

void Magneto_init()										/* Magneto initialize function */
{
	I2C_Start(0x3C);									/* Start and write SLA+W */
	I2C_Write(0x00);									/* Write memory location address */
	I2C_Write(0x70);									/* Configure register A as 8-average, 15 Hz default, normal measurement */
	I2C_Write(0xA0);									/* Configure register B for gain */
	I2C_Write(0x00);									/* Configure continuous measurement mode in mode register */
	I2C_Stop();											/* Stop I2C */
}

int Magneto_GetHeading()
{
	int x, y, z;
	double Heading;
	I2C_Start_Wait(0x3C);								/* Start and wait for acknowledgment */
	I2C_Write(0x03);									/* Write X register address */
	I2C_Repeated_Start(0x3D);							/* Generate repeat start condition with SLA+R */
	/* Read 16 bit x,y,z value (2’s complement form) */
	x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();											/* Stop I2C */
	Heading = atan2((double)y,(double)x) + Declination;
	if (Heading>2*PI)									/* Due to declination check for >360 degree */
		Heading = Heading - 2*PI;
	if (Heading<0)										/* Check for sign */
		Heading = Heading + 2*PI;
	return (Heading* 180 / PI);							/* Convert into angle and return */
}

/* INIT USART (RS-232)  */
void USART_init( unsigned int ubrr )
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);     // Enable RX, TX & RX interrupt
	UCSR0C = (3 << UCSZ00);    //asynchronous 8 N 1
}
/* SEND A STRING TO THE RS-232 */
void USART_tx_string( char *data )
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

#define degree_sysmbol 0xdf

int main(void)
{
	char buffer[10];
   USART_init(BAUD_PRESCALLER);  // Initialize the USART
   USART_tx_string("Connected!\r\n");    // we're alive!
   _delay_ms(125);         // wait a bit
	I2C_Init();											/* Initialize I2C */
	Magneto_init();										/* Initialize magneto */
	USART_tx_string("Magnetometer");							/* Print title on LCD */
	while (1)
	{
		itoa (Magneto_GetHeading(),buffer,10);
		USART_tx_string("Heading = ");				/* Print Heading on 2nd row of LCD */
		sprintf(buffer,"%d%cC  \r\n", (int)Magneto_GetHeading(), degree_sysmbol);               /* convert integer value to ASCII string */
		USART_tx_string(buffer);
	}
}