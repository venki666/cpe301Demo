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
#define Si7021 (0x40<<1)     // (0x68 << 1) I2C slave address

unsigned char ret;        // return value

uint16_t raw;             // raw sensor value
uint16_t HT_Data, CHT_Data;          // Humidity raw value
uint16_t TMP_Data, CTMP_Data;          // Temperature raw value
uint16_t PTMP_Data, CPTMP_Data;          // Temperature raw value

void Si7021_writereg(uint8_t reg, uint8_t val)
{
	i2c_start(Si7021+I2C_WRITE);
	i2c_write(reg);  // go to register e.g. 106 user control
	i2c_write(val);  // set value e.g. to 0100 0000 FIFO enable
	i2c_stop();      // set stop condition = release bus
}


uint16_t Si7021_readreg(uint8_t reg)
{
	i2c_start_wait(Si7021+I2C_WRITE);  // set device address and write mode
	i2c_write(reg);                     // ACCEL_XOUT
	i2c_write(0x81);                     // ACCEL_XOUT
	i2c_rep_start(Si7021+I2C_READ);    // set device address and read mode
	raw = i2c_readAck();                // read one intermediate byte
	raw = (raw<<8) | i2c_readNak();     // read last byte
	i2c_stop();
	return raw;
}

void Init_Si7021()
{
	i2c_init();     // init I2C interface
	_delay_ms(200);  // Wait for 200 ms.

	ret = i2c_start(Si7021+I2C_WRITE);       // set device address and write mode
	if ( ret )
	{
		/* failed to issue start condition, possibly no device found */
		i2c_stop();
		//red_led(1);    // Turn on the red LED immediately to signal the read is not ok
		while(1) {;;}  // lock program here as sensor init failed
	}
	else
	{
	}
}

int main(void)
{
	char buffer[20], float_[15];
	USART_Init(9600);									/* Initialize USART with 9600 baud rate */

	sei();             // Enable global interrupts

	Init_Si7021();    // Sensor init
	_delay_ms(200);     // Wait for 200 ms.

	while (1)
	{

	TMP_Data = Si7021_readreg(0xE3);   // read ch0 LSB byte
	HT_Data = Si7021_readreg(0xE5);   // read ch0 LSB byte
	PTMP_Data = Si7021_readreg(0xE0);   // read ch0 LSB byte
	CHT_Data = 125.0f * HT_Data / 65536.0f - 6.0f;
	CTMP_Data = 175.25f * TMP_Data / 65536.0f - 46.85f;
	CPTMP_Data = 175.25f * PTMP_Data / 65536.0f - 46.85f;
	dtostrf( CTMP_Data, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
	sprintf(buffer," Temperature = %s C\n",float_);
	USART_SendString(buffer);
	dtostrf( CHT_Data, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
	sprintf(buffer," Relative Humidity = %s RH\n",float_);
	USART_SendString(buffer);
	dtostrf( CPTMP_Data, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
	sprintf(buffer," P Temperature = %s C\n",float_);
	USART_SendString(buffer);

	}
}