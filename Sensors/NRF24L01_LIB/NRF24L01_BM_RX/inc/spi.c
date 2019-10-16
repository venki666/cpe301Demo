/*  Copyright (c) 2015 B. Sidhipong <bsidhipong@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <avr/common.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"

#define DDR_SPI		DDRB
#define DD_MOSI		DDB3
#define DD_MISO		DDB4
#define DD_SCK		DDB5

void spi_master_init( void )
{
	DDR_SPI &= ~_BV(DD_MISO);
	DDR_SPI |= (_BV(DD_MOSI) | _BV(DD_SCK));
	SPCR = (0 << SPIE) |
	       (1 << SPE)  |
	       (0 << DORD) |
	       (1 << MSTR) |
	       (0 << CPOL) |
	       (0 << SPR1) | (0 << SPR0); //fosc/2
	SPSR |= _BV(SPI2X);
}

void spi_bulk_send( uint8_t *send_buffer, uint8_t count )
{
	while ( count-- ) {
		SPDR = *send_buffer++;
		loop_until_bit_is_set(SPSR, SPIF);
	}
}

void spi_send( uint8_t send_data )
{
	SPDR = send_data;
	loop_until_bit_is_set(SPSR, SPIF);
}

void spi_bulk_exchange( uint8_t *send_buffer, uint8_t *receive_buffer, uint8_t count )
{
	while ( count-- ) {
		SPDR = *send_buffer++;
		loop_until_bit_is_set(SPSR, SPIF);
		*receive_buffer++ = SPDR;
	}
}

uint8_t spi_exchange( uint8_t send_data )
{
	SPDR = send_data;
	loop_until_bit_is_set(SPSR, SPIF);
	return SPDR;
}
