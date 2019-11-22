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
#include <util/twi.h>

#include "i2cmaster.h"
#include "uart.h"					/* Include USART header file */
#define Si7021 (0x40<<1)     // (0x40 << 1) I2C slave address

#define Si7021_Measure_RH_Hold_Mode             0xE5
#define Si7021_Measure_RH_No_Hold_Mode          0xF5
#define Si7021_Measure_T_Hold_Mode              0xE3
#define Si7021_Measure_T_No_Hold_Mode           0xF3
#define Si7021_Read_T_from_Last_RH_Value        0xE0
#define Si7021_Reset                            0xFE
#define Si7021_Write_Config_Register            0xE6
#define Si7021_Read_Config_Register             0xE7
#define Si7021_Write_Heater_Control_Register    0x51
#define Si7021_Read_Heater_Control_Register     0x11
#define Si7021_Read_ID1_H                       0xFA
#define Si7021_Read_ID1_L                       0x0F
#define Si7021_Read_ID2_H                       0xFC
#define Si7021_Read_ID2_L                       0xC9
#define Si7021_Read_Firmware_Version_H          0x84
#define Si7021_Read_Firmware_Version_L          0xB8

#define Si7021_Config_Resolution_RH_12_T14      0x00
#define Si70211_Config_Resolution_RH_8_T12      0x01
#define Si70211_Config_Resolution_RH_10_T13     0x80
#define Si70211_Config_Resolution_RH_11_T11     0x81

#define Si70211_Config_VDD_Ok                   0x00
#define Si70211_Config_VDD_Low                  0x40

#define Si70211_Config_Heater_Off               0x00
#define Si70211_Config_Heater_On                0x04

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

void Si7021_config(void)
{
	Si7021_writereg(Si7021_Write_Heater_Control_Register, 0x09);
	Si7021_writereg(Si7021_Write_Config_Register, (Si7021_Config_Resolution_RH_12_T14 | Si70211_Config_Heater_On));
	_delay_ms(900);
	Si7021_writereg(Si7021_Write_Heater_Control_Register, 0x00);
	Si7021_writereg(Si7021_Write_Config_Register, (Si7021_Config_Resolution_RH_12_T14 | Si70211_Config_Heater_Off));
	_delay_ms(900);
}

int main(void)
{
	char buffer[30], float_[5];
    USART_Init(9600);
	sei();             // Enable global interrupts

	Init_Si7021();    // Sensor init
	Si7021_config();
	_delay_ms(200);     // Wait for 200 ms.

	while (1)
	{
	TMP_Data = Si7021_readreg(0xE3);   // read ch0 LSB byte
	CTMP_Data = 175.25f * TMP_Data / 65536.0f - 46.85f;
	dtostrf( CTMP_Data, 3, 2, float_ );					
	sprintf(buffer," Temperature = %s C\n",float_);
	USART_SendString(buffer);
	_delay_ms(2000);     // Wait for 2000 ms.
	}
}
