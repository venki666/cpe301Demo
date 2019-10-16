/*
 * Sensor_ATMega328P.c
 *
 * Created: 4/7/2019 1:38:54 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL									/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>										/* Include AVR std. library file */
#include <util/delay.h>									/* Include delay header file */
#include <inttypes.h>									/* Include integer type header file */
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include <avr/interrupt.h>			/* Include avr interrupt header file */
#include "i2cmaster.h"
#include "uart.h"							/* Include USART header file */
#define MPU6050  0xD0     // (0x68 << 1) I2C slave address

unsigned char ret;        // return value

uint16_t raw;             // raw sensor value
uint16_t acrawx;          // x axis acceleration raw value


void MPU6050_writereg(uint8_t reg, uint8_t val)
{
	i2c_start(MPU6050+I2C_WRITE);
	i2c_write(reg);  // go to register e.g. 106 user control
	i2c_write(val);  // set value e.g. to 0100 0000 FIFO enable
	i2c_stop();      // set stop condition = release bus
}


uint16_t MPU6050_readreg(uint8_t reg)
{
	i2c_start_wait(MPU6050+I2C_WRITE);  // set device address and write mode
	i2c_write(reg);                     // ACCEL_XOUT
	i2c_rep_start(MPU6050+I2C_READ);    // set device address and read mode
	raw = i2c_readAck();                // read one intermediate byte
	raw = (raw<<8) | i2c_readNak();     // read last byte
	i2c_stop();
	
	return raw;
}


void Init_MPU6050()
{
	i2c_init();     // init I2C interface
	_delay_ms(200);  // Wait for 200 ms.

	ret = i2c_start(MPU6050+I2C_WRITE);       // set device address and write mode
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
		MPU6050_writereg(0x6B, 0x00); // go to register 107 set value to 0000 0000 and wake up sensor
		MPU6050_writereg(0x19, 0x08); // go to register 25 sample rate divider set value to 0000 1000 for 1000 Hz
		MPU6050_writereg(0x1C, 0x08); // go to register 28 acceleration configuration set value to 0000 1000 for 4g
		MPU6050_writereg(0x23, 0xF8); // go to register 35 FIFO enable set value to 1111 1000 for all sensors not slave
		MPU6050_writereg(0x37, 0x10); // go to register 55 interrupt configuration set value to 0001 0000 for logic level high and read clear
		MPU6050_writereg(0x38, 0x01); // go to register 56 interrupt enable set value to 0000 0001 data ready creates interrupt
		MPU6050_writereg(0x6A, 0x40); // go to register 106 user control set value to 0100 0000 FIFO enable
	}
}

int main(void)
{
	float Xa;
	char buffer[20], float_[10];
	USART_Init(9600);									/* Initialize USART with 9600 baud rate */
	acrawx = 0;        // initial value, without read led will not blink

	sei();             // Enable global interrupts

	Init_MPU6050();    // Sensor init
	_delay_ms(200);     // Wait for 200 ms.

	while (1)
	{
	acrawx = MPU6050_readreg(0x3B);   // read raw X acceleration from fifo
	Xa = acrawx/8192.0;
	dtostrf( Xa, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
	sprintf(buffer," Ax = %s g\n",float_);
	USART_SendString(buffer);
	}
}