/*
 * Sensor_ATMega328P.c
 *
 * Created: 4/7/2019 1:38:54 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL			/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>					/* Include AVR std. library file */
#include <util/delay.h>				/* Include delay header file */
#include <inttypes.h>				/* Include integer type header file */
#include <stdlib.h>					/* Include standard library file */
#include <stdio.h>					/* Include standard library file */
#include <avr/interrupt.h>			/* Include avr interrupt header file */
#include "i2cmaster.h"
#include "uart.h"					/* Include USART header file */
#define TSL2591  (0x29<<1)     // (0x68 << 1) I2C slave address

unsigned char ret;        // return value

uint16_t raw;             // raw sensor value
uint16_t CH0_Data;          // CH0 raw value


void TSL2591_writereg(uint8_t reg, uint8_t val)
{
	i2c_start(TSL2591+I2C_WRITE);
	i2c_write(reg);  // go to register e.g. 106 user control
	i2c_write(val);  // set value e.g. to 0100 0000 FIFO enable
	i2c_stop();      // set stop condition = release bus
}


uint16_t TSL2591_readreg(uint8_t reg)
{
	i2c_start_wait(TSL2591+I2C_WRITE);  // set device address and write mode
	i2c_write(reg);                     // ACCEL_XOUT
	i2c_rep_start(TSL2591+I2C_READ);    // set device address and read mode
	raw = i2c_readAck();                // read one intermediate byte
	raw = (raw<<8) | i2c_readNak();     // read last byte
	i2c_stop();
	
	return raw;
}


void Init_TSL2591()
{
	i2c_init();     // init I2C interface
	_delay_ms(200);  // Wait for 200 ms.

	ret = i2c_start(TSL2591+I2C_WRITE);       // set device address and write mode
	if ( ret )
	{
		/* failed to issue start condition, possibly no device found */
		i2c_stop();
		//red_led(1);    // Turn on the red LED immediately to signal the read is not ok
		while(1) {;;}  // lock program here as sensor init failed
	}
	else
	{
		/* issuing start condition ok, device accessible */
		// Select control register(0x00 | 0x80)
		// Power ON mode(0x01)
		TSL2591_writereg(0x00|0x80, 0x01); 
		// Select timing register(0x01 | 0x80)
		// Nominal integration time = 200ms(001)
		TSL2591_writereg(0x01|0x80, 0x01); 
	}
}



int main(void)
{
	char buffer[20], float_[10];
	USART_Init(9600);									/* Initialize USART with 9600 baud rate */
	sei();             // Enable global interrupts

	Init_TSL2591();    // Sensor init
	_delay_ms(200);     // Wait for 200 ms.

	while (1)
	{
	CH0_Data = TSL2591_readreg(0x14|0x80);   // read ch0 LSB byte
	dtostrf( CH0_Data, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
	sprintf(buffer," CH0 Data = %s\n",float_);
	USART_SendString(buffer);
	}
}