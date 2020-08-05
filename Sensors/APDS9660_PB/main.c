#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

#include "i2cmaster.h"
#include "uart.h"					/* Include USART header file */

#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73 

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

char buffer[6];

int16_t raw_R = 0;
int16_t raw_G = 0;
int16_t raw_B = 0;
int16_t raw_C = 0;;

void init_APDS9960(void){
	_delay_ms(150);										/* Power up time >100ms */
	i2c_start(APDS9960_WRITE);
	i2c_write(0x80); // set pointer to ENABLE REGISTER
	i2c_write(0x03); // write 0x06 to ENABLE REGISTER
	i2c_stop();

	i2c_start(APDS9960_WRITE);
	i2c_write(0x81); // set pointer to ALS ADC Integration Time
	i2c_write(0xDB); // 219 =0XDB = 37 = 103 ms = 37889
	i2c_stop();

	i2c_start(APDS9960_WRITE);
	i2c_write(0x8F); // set pointer to CONTROL<AGAIN>
	i2c_write(0x01); // ALS and Color Gain Control = 0 = x1
	i2c_stop();
}

void getreading(void){

	i2c_start(APDS9960_WRITE);
	i2c_write(0x94); // set pointer to CDATA
	i2c_stop();

	i2c_start(APDS9960_READ);

	raw_C =  ((uint8_t)i2c_readAck());
	raw_C |= ((uint8_t)i2c_readAck())<<8;

	raw_R = ((uint8_t)i2c_readAck());
	raw_R |= ((uint8_t)i2c_readAck())<<8;

	raw_G = ((uint8_t)i2c_readAck());
	raw_G |= ((uint8_t)i2c_readAck())<<8;

	raw_B = ((uint8_t)i2c_readAck());
	raw_B |= ((uint8_t)i2c_readAck())<<8;

	i2c_stop();

}

int main(void){
	
	init_uart(9600);
	i2c_init();
	init_APDS9960();
	
	while(1){
		getreading();
		
		itoa(raw_C, buffer, 10);
		uart_puts(buffer);
		uart_puts("  ");
		
		itoa(raw_R, buffer, 10);
		uart_puts(buffer);
		uart_puts("  ");
		
		itoa(raw_G, buffer, 10);
		uart_puts(buffer);
		uart_puts("  ");
		
		itoa(raw_B, buffer, 10);
		uart_puts(buffer);
		uart_puts("  \r\n");
		
		_delay_ms(1000);
	}
	
	return 0;
}
