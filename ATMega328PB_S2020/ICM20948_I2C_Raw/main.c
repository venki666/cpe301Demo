/*
 * ICM20948_I2C_Raw.c
 *
 * Created: 7/6/2021 3:40:03 PM
 * Author : VenkatesanMuthukumar
 */ 

#define F_CPU 16000000UL //define CPU clock frequency
#define SCL_CLK 100000L //define SCL clock frequency
#define TW_STATUS_PB (TWSR0 & 0xF8)
#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 16UL))) - 1)	// Baud rate

#include <avr/io.h>
#include <util/delay.h>				// Include delay header file
#include <inttypes.h>				// Includes integer type header file
#include <stdlib.h>					// Include standard library file
#include <stdio.h>					// Include standard library file
#include <avr/interrupt.h>			// Include avr interrupt header file
#include <util/twi.h>
#include <math.h>					// Includes math header file

#include "icm20948_def.h"
#include "i2cmaster.h"
#include "uart.h"

/*Global Variables*/
uint8_t ret;									// return value
uint16_t raw;									// raw sensor value
uint16_t ACC_Data[3], GYRO_Data[3];			// raw values
volatile float pitch, roll;
volatile float pitchAcc, rollAcc;

/************************************************************************/
/*                      Functions for ICM20948 chip                      */
/************************************************************************/

/*writes to specific register a user chosen value*/
void ICM_writereg(uint8_t reg, uint8_t val)
{
	i2c_start(ICM20948+I2C_WRITE);		//device address and write mode
	i2c_write(reg);						//go to register
	i2c_write(val);						//set value
	i2c_stop();							//set stop condition
}

/*reads from a specific register*/
uint8_t ICM_readreg(uint8_t reg)
{
	i2c_start(ICM20948+I2C_WRITE);		//device address and write mode
	i2c_write(reg);                     //ACCEL_XOUT
	i2c_stop();							//set stop condition
	_delay_ms(100);
	i2c_start(ICM20948+I2C_READ);		//device address and read mode
	raw = i2c_readNak();                //read one byte
	i2c_stop();
	return raw;
}

/*reads the slave device*/
void ICM_get_whom_am_I()
{
	uint8_t data[1];
	ICM_writereg(ICM20948_REG_PWR_MGMT_1, 0x80);		//clear data
	_delay_ms(10);
	ICM_writereg(ICM20948_REG_PWR_MGMT_1, 0x01);		//set clock
	_delay_ms(10);  // Wait for 200 ms.
	data[0]=ICM_readreg(ICM20948_REG_WHO_AM_I);			//check slave device
	if (data[0] != ICM20948_DEVICE_ID)					//inform if device found
	USART_SendString("Device Not Found\n");
	else
	USART_SendString("Device Found\n");
}

/*Initializes the ICM20948 device*/
void Init_ICM()
{
	i2c_init();											// initialize I2C interface
	USART_SendString("I2C Initialized\n");
	_delay_ms(2);										// Wait for 200 ms.
	ICM_get_whom_am_I();
}

#define ACCELEROMETER_SENSITIVITY 16384.0
#define GYROSCOPE_SENSITIVITY 16.4
#define dt 0.01							// 10 ms sample rate!

void ComplementaryFilter()
{

	// Integrate the gyroscope data -> int(angularSpeed) = angle
	pitch += ((float)GYRO_Data[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
	roll -= ((float)GYRO_Data[1] / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis
	
	// Compensate for drift with accelerometer data if !bullshit
	// Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
	int forceMagnitudeApprox = abs(ACC_Data[0]) + abs(ACC_Data[1]) + abs(ACC_Data[2]);
	if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
	{
		// Turning around the X axis results in a vector on the Y-axis
		pitchAcc = atan2f((float)ACC_Data[1], (float)ACC_Data[2]) * 180 / M_PI;
		pitch = pitch * 0.98 + pitchAcc * 0.02;
		
		// Turning around the Y axis results in a vector on the X-axis
		rollAcc = atan2f((float)ACC_Data[0], (float)ACC_Data[2]) * 180 / M_PI;
		roll = roll * 0.98 + rollAcc * 0.02;
	}
}


int main(void)
{
	/*Variables*/
	//arrays to hold answers
	char buffer[30];
	char float_[6];
	
	//variables for accelerometer and gyroscope
	USART_Init(9600);
	USART_SendString("UART Initialized\n");
	
	Init_ICM();    // Sensor initialize
	_delay_ms(2);		// Wait for 200 ms.
	
	while(1)
	{
		//storing accelerometer and gyroscope values
		ACC_Data[0] = ((ICM_readreg(ICM20948_REG_ACCEL_XOUT_H_SH)<<8) | ICM_readreg(ICM20948_REG_ACCEL_XOUT_L_SH))/16384.0;
		ACC_Data[1] = ((ICM_readreg(ICM20948_REG_ACCEL_YOUT_H_SH)<<8) | ICM_readreg(ICM20948_REG_ACCEL_YOUT_L_SH))/16384.0;
		ACC_Data[2] = ((ICM_readreg(ICM20948_REG_ACCEL_ZOUT_H_SH)<<8) | ICM_readreg(ICM20948_REG_ACCEL_ZOUT_L_SH))/16384.0;
		GYRO_Data[0] = ((ICM_readreg(ICM20948_REG_GYRO_XOUT_H_SH)<<8) | ICM_readreg(ICM20948_REG_GYRO_XOUT_L_SH))/131;
		GYRO_Data[1] = ((ICM_readreg(ICM20948_REG_GYRO_YOUT_H_SH)<<8) | ICM_readreg(ICM20948_REG_GYRO_YOUT_L_SH))/131;
		GYRO_Data[2] = ((ICM_readreg(ICM20948_REG_GYRO_ZOUT_H_SH)<<8) | ICM_readreg(ICM20948_REG_GYRO_ZOUT_L_SH))/131;
		
		//printing accelerometer and gyroscope values
		dtostrf(ACC_Data[0], 4, 2, float_);
		snprintf(buffer, sizeof(buffer), "Ax = %s g\t", float_);
		USART_SendString(buffer);
	}
}

