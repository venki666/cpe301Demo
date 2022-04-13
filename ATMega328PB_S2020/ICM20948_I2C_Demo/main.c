/*
 * ICM20948_I2C_Demo.c
 *
 * Created: 1/16/2022 12:30:06 AM
 * Author : venkim
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "i2cmaster.h"

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) -1)

#define ICM20948	(0x68<<1)     // (1101001 << 1) I2C slave address when AD0=1
#define WHO_AM_I	 0x00
#define PWR_MGMT_1	 0x06
#define ACCEL_XOUT_H 0x2D
#define ACCEL_YOUT_H 0x2F
#define ACCEL_ZOUT_H 0x31
#define GYRO_XOUT_H	 0x33
#define GYRO_YOUT_H	 0x35
#define GYRO_ZOUT_H	 0x37

#define DEVICE_ID	 0xEA

uint8_t raw;
uint16_t bigraw;

void USART_init(unsigned int ubrr);
void USART_tx_string(char * data);

//void icm_start_loc(void);
//void read_display_icm(void);

void ICM20948_writereg(uint8_t reg, uint8_t val);
	
/* functions i modified*/
uint16_t ICM20948_readreg16(uint8_t reg);
void ICM20948_Init(void);
void ICM20948_verify_whoami(void);

char buffer[30], myfloat[5];



int main(void)
{
	PORTC |= (1<<5) | (1<<4);			// enable pull ups for TWI pins
	

	i2c_init();							// initialize TWI
	USART_init(BAUD_PRESCALLER);		// initialize USART
	USART_tx_string("UART Connected!\r\n");
    
	ICM20948_Init();				// change clksel on icm
	ICM20948_verify_whoami();			// verify we are connected
	
	_delay_ms(200);

	while(1){
/*		accel_x = ICM20948_readreg16(ACCEL_XOUT_H);
		accel_y = ICM20948_readreg16(ACCEL_YOUT_H);
		accel_z = ICM20948_readreg16(ACCEL_ZOUT_H);
		gyro_x = ICM20948_readreg16(GYRO_XOUT_H);
		gyro_y = ICM20948_readreg16(GYRO_YOUT_H);
		gyro_z = ICM20948_readreg16(GYRO_ZOUT_H);
		
		snprintf(buffer,sizeof(buffer),"ACCEL X: %d\r\n",accel_x);
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"ACCEL Y: %d\r\n",accel_y);
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"ACCEL Z: %d\r\n",accel_z);
		USART_tx_string(buffer);
		
		USART_tx_string("\r\n");
		
		snprintf(buffer,sizeof(buffer),"GYRO X: %d\r\n",gyro_x);
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"GYRO Y: %d\r\n",gyro_y);
		USART_tx_string(buffer);
		
		snprintf(buffer,sizeof(buffer),"GYRO Z: %d\r\n",gyro_z);
		USART_tx_string(buffer);
		
		USART_tx_string("\r\n");
		//read_display_icm();
		_delay_ms(200); */
	}
}

void USART_init(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

void USART_tx_string(char * data){
	while((*data != '\0')){
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

/* Ensure we are talking to ICM, print WHO_AM_I to terminal, should be 0xEA or 234 */
void ICM20948_verify_whoami(void){
	uint8_t who_am_i = 0;
	i2c_start(ICM20948+I2C_WRITE);		// 68 << 1 = D0
	i2c_write(WHO_AM_I);				// select who_am_i
	i2c_stop();							// halt i2c
	i2c_start(ICM20948+I2C_READ);		// D0 + 1(TWI READ)
	who_am_i = i2c_readNak();			// save to variable
	snprintf(buffer,sizeof(buffer),"DEVICE ID: %02x\r\n",who_am_i);
	USART_tx_string(buffer);
	i2c_stop();
}

/* Change clksel to use best available clock source  */
void ICM20948_Init(void){
	i2c_start(0xD0);					// select ICM20948 (0x68<<1)+0
	i2c_write(0x06);					// select pwr_mgmt_1
	i2c_write(0x01);					// set bit 1
	i2c_stop();
}

/* modified to writes to and reads from reg+1 */
uint16_t ICM20948_readreg16(uint8_t reg)
{
	i2c_start(ICM20948+I2C_WRITE);		// set device address and write mode
	i2c_write(reg);						// ACCEL_XOUT
	i2c_start(ICM20948+I2C_READ);		// set device address and read mode
	raw = i2c_readNak();                // read one intermediate byte
	i2c_start(ICM20948+I2C_WRITE);
	i2c_write(reg + 1);
	i2c_start(ICM20948+I2C_READ);
	bigraw = (raw<<8) | i2c_readNak();  // read last byte
	i2c_stop();
	return bigraw;
}
