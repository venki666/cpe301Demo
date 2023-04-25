/*
 * ATmega16 Interface with MPU-6050
 * http://www.electronicwings.com
 *
 */ 


#define F_CPU 16000000UL									/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>										/* Include AVR std. library file */
#include <util/delay.h>									/* Include delay header file */
#include <inttypes.h>									/* Include integer type header file */
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include "MPU6050_def.h"							/* Include MPU6050 register define file */
#include "i2c_master.h"							/* Include I2C Master header file */
#include "uart.h"							/* Include USART header file */

//float Acc_x,Acc_y,Acc_z,Temperature,Gyro_x,Gyro_y,Gyro_z;
volatile float Temperature;
volatile int accData[3], gyrData[3];
volatile float pitch, roll;

void MPU6050_Init()										/* Gyro initialization function */
{
	_delay_ms(150);										/* Power up time >100ms */
	I2C_Start_Wait(0xD0);								/* Start with device write address */
	I2C_Write(SMPLRT_DIV);								/* Write to sample rate register */
	I2C_Write(0x07);									/* 1KHz sample rate */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);								/* Write to power management register */
	I2C_Write(0x01);									/* X axis gyroscope reference frequency */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);									/* Write to Configuration register */
	I2C_Write(0x00);									/* Fs = 8KHz */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);								/* Write to Gyro configuration register */
	I2C_Write(0x18);									/* Full scale range +/- 2000 degree/C */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);								/* Write to interrupt enable register */
	I2C_Write(0x01);
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);								/* I2C start with device write address */
	I2C_Write(ACCEL_XOUT_H);							/* Write start location address from where to read */ 
	I2C_Repeated_Start(0xD1);							/* I2C start with device read address */
}

void Read_RawValue()
{
	MPU_Start_Loc();									/* Read Gyro values */
	accData[0] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	accData[1] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	accData[2] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Temperature = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	gyrData[0] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	gyrData[1] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	gyrData[2] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
}

#define ACCELEROMETER_SENSITIVITY 16384.0
#define GYROSCOPE_SENSITIVITY 16.4
#define dt 0.01							// 10 ms sample rate!

void ComplementaryFilter()
{
	float pitchAcc, rollAcc;
	
	// Integrate the gyroscope data -> int(angularSpeed) = angle
	pitch += ((float)gyrData[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
	roll -= ((float)gyrData[1] / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis
	
	// Compensate for drift with accelerometer data if !bullshit
	// Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
	int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1]) + abs(accData[2]);
	if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
	{
		// Turning around the X axis results in a vector on the Y-axis
		pitchAcc = atan2f((float)accData[1], (float)accData[2]) * 180 / M_PI;
		pitch = pitch * 0.98 + pitchAcc * 0.02;
		
		// Turning around the Y axis results in a vector on the X-axis
		rollAcc = atan2f((float)accData[0], (float)accData[2]) * 180 / M_PI;
		roll = roll * 0.98 + rollAcc * 0.02;
	}
}

int main()
{
	char buffer[20], float_[10];
	I2C_Init();											/* Initialize I2C */
	MPU6050_Init();										/* Initialize MPU6050 */
	USART_Init(9600);									/* Initialize USART with 9600 baud rate */
	
	while(1)
	{
		Read_RawValue();
		ComplementaryFilter();

		dtostrf(pitch, 3, 2, float_ );					/* Take values in buffer to send all parameters over USART */
		sprintf(buffer,"%s, ",float_);
		USART_SendString(buffer);
		
		dtostrf(roll, 3, 2, float_ );
		sprintf(buffer,"%s\r\n",float_);
		USART_SendString(buffer);
	}
}
