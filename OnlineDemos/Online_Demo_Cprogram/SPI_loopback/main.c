/*
 * SPI_loopback.c
 *
 * Created: 2/25/2019 6:12:47 PM
 * Author : VenkatesanMuthukumar
 */

/* SPI Test program
 * Designed to be used with a jumper from MOSI to MISO for loopback test
 * uC is Atmel ATmega128
 */

#define F_CPU 16000000UL

#define BAUD_RATE 9600
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

unsigned char SPI_transmit(unsigned char);
unsigned char SPI_receive(void);

void usart_init();
void usart_send(unsigned char ch);
void USART_putstring(char *StringPtr);

unsigned char data = 'a'; //0X41;

int main() {
  DDRB |= (1 << 2) | (1 << 3) | (1 << 5); // SCK, MOSI and SS as outputs
  PORTB &= (0<<2);

  // USART0 initialization
  usart_init();

  // initialize SPI with lowest frequency possible for debug purposes
  //	SPCR = 0b01110011;
  SPCR = (0 << SPIE) | // SPI Interrupt Enable
         (1 << SPE) |  // SPI Enable
         (0 << DORD) | // Data Order: MSB first
         (1 << MSTR) | // Master mode
         (0 << CPOL) | // Clock Polarity:
         (0 << CPHA) | // Clock Phase:
         (1 << SPR1) | // Clock Frequency: f_OSC / 4
         (1 << SPR0);
  SPSR |= (0 << SPI2X); // Doubled Clock Frequency: f_OSC / 2
  for (;;) {
    SPI_transmit(data);
    USART_putstring("TX Complete  ...   ");
    _delay_ms(200);
    usart_send(SPI_receive());
    USART_putstring("    ..... RX Complete");
    USART_putstring("\n");
  }
}

void usart_init(void) {
  UCSR0B = (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  UBRR0L = F_CPU / 16 / BAUD_RATE - 1;
}

void usart_send(unsigned char ch) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;        // wait until UDR0 is empty
  UDR0 = ch; // transmit ch
}

void USART_putstring(char *StringPtr) {

  while (*StringPtr != 0x00) {
    usart_send(*StringPtr);
    StringPtr++;
  }
}

unsigned char SPI_transmit(unsigned char data) {
  // Start transmission
  SPDR = data;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)))
    ;
  data = SPDR;
  return (data);
}
unsigned char SPI_receive(void) {
  unsigned char data;
  // Wait for reception complete
  SPDR = 0xff;
  while (!(SPSR & (1 << SPIF)))
    ;
  data = SPDR;
  return data;
}
