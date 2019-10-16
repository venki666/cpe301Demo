/*
 * I2C_Master_H_file.h
 * http://www.electronicwings.com
 *
 */ 


#ifndef I2C_MASTER_H_FILE_H_					/* Define library H file if not defined */
#define I2C_MASTER_H_FILE_H_

#define F_CPU 16000000UL							/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>								/* Include AVR std. library file */
#include <util/delay.h>							/* Include delay header file */
#include <math.h>								/* Include math function */
#define SCL_CLK 100000L							/* Define SCL clock frequency */
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1))))) /* Define bit rate */

void I2C_Init();								/* I2C initialize function */
uint8_t  I2C_Start(char);						/* I2C start function */
uint8_t  I2C_Repeated_Start(char);				/* I2C repeated start function */
void I2C_Stop();								/* I2C stop function */
void I2C_Start_Wait(char);						/* I2C start wait function */
uint8_t  I2C_Write(char);						/* I2C write function */
char I2C_Read_Ack();							/* I2C read ack function */
char I2C_Read_Nack();							/* I2C read nack function */


#endif											/* I2C_MASTER_H_FILE_H_ */