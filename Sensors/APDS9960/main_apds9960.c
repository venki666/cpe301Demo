/*
 * ATmega328p Interface with APDS9960
 *
 */ 


#define F_CPU 16000000UL									/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>										/* Include AVR std. library file */
#include <util/delay.h>									/* Include delay header file */
#include <inttypes.h>									/* Include integer type header file */
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include "APDS9960_def.h"							/* Include APDS9960 register define file */
#include "i2c_master.h"							/* Include I2C Master header file */
#include "uart.h"							/* Include USART header file */

// Constants
#define LIGHT_INT_HIGH  1000 // High light level for interrupt
#define LIGHT_INT_LOW   10   // Low light level for interrupt

// Global variables

uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
int isr_flag = 0;
uint16_t threshold = 0;

void init_uart(uint16_t baudrate){

	uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);

	UBRR0H = UBRR_val >> 8;
	UBRR0L = UBRR_val;

	UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); // UART TX (Transmit - senden) einschalten
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00); //Modus Asynchron 8N1 (8 Datenbits, No Parity, 1 Stopbit)
}

void uart_putc(unsigned char c){

	while(!(UCSR0A & (1<<UDRE0))); // wait until sending is possible
	UDR0 = c; // output character saved in c
}

void uart_puts(char *s){
	while(*s){
		uart_putc(*s);
		s++;
	}
}

void APDS9960_Init()										/* Gyro initialization function */
{
	_delay_ms(150);										/* Power up time >100ms */
	I2C_Start_Wait(0x72);								/* Start with device write address */
	I2C_Write(0X80);								/* Write to sample rate register */
	I2C_Write(0x03);									/* Enable Power & ALS */
	I2C_Stop();

	I2C_Start_Wait(0x72);
	I2C_Write(0X81);								/* Write to power management register */
	I2C_Write(0XB6);									/*  */
	I2C_Stop();

	I2C_Start_Wait(0x72);
	I2C_Write(0X8F);									/* Write to Configuration register */
	I2C_Write(0X00);									/*   */
	I2C_Stop();

}

void APDS9960_Start_Loc()
{
	I2C_Start_Wait(0x72);								/* I2C start with device write address */
	I2C_Write(APDS9960_CDATAL);							/* Write start location address from where to read */ 
	I2C_Stop();

	I2C_Start_Wait(0X73);
}

void APDS9960_Read_RawValue()
{
	APDS9960_Start_Loc();									/* Read Gyro values */
	ambient_light = (((uint8_t)I2C_Read_Ack()<<8) | I2C_Read_Ack());
	red_light = (((uint8_t)I2C_Read_Ack()<<8) | I2C_Read_Ack());
	green_light = (((uint8_t)I2C_Read_Ack()<<8) | I2C_Read_Ack());
	blue_light = (((uint8_t)I2C_Read_Ack()<<8) | I2C_Read_Ack());
	I2C_Stop();
}

int main()
{
	char buffer[10];
	I2C_Init();											/* Initialize I2C */
	APDS9960_Init();										/* Initialize APDS9960 */
	USART_Init(9600);									/* Initialize USART with 9600 baud rate */
	
	while(1)
	{
		APDS9960_Read_RawValue();
		
		itoa(ambient_light, buffer, 10);
		uart_puts(buffer);
		uart_puts("  ");

	}
}
