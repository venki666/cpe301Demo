/*
 * main.c
 *
 *  Created on: Jan 6, 2015
 *      Author: mikedanylov
 */

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "NRF24L01/nrf24.h"
#include "HD44780/hd44780.h"
#include "sensor/Sensor.h"
#include "uart/uart.h"

#define PAYLOAD 8
#define NRF_CH 2
#define UART_BAUD_RATE 4800

int main(){

	uint8_t nrf_buff[PAYLOAD];
	uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
	sei();

	nrf24_init();
	nrf24_config(NRF_CH, PAYLOAD);
	nrf24_rx_address(rx_address);

	while(1){
		if(nrf24_dataReady()){

			nrf24_getData(nrf_buff);
			uart_puts(nrf_buff);
			uart_puts("\r\n");
			_delay_ms(10);
		}
	}
}
