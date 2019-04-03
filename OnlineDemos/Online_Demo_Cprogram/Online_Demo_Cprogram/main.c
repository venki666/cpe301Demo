#include <avr/io.h>
#include <stdio.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif
#include <stdlib.h>
#include <util/delay.h>
#include <util/setbaud.h>

#define LATCH 4 /* PD4 RCK */
#define CLOCK 7 /* PD7 SRCK */
#define DATA 0  /* PB0 SER IN */
#define LSBFIRST 0
#define MSBFIRST 1

/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
                            0x92, 0x82, 0xF8, 0X80, 0X90};
/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};

void shift_out_init(void) {
  DDRB |= (1 << DATA);
  DDRD |= (1 << CLOCK) | (1 << LATCH);
}
void uart_init(void) {
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~(_BV(U2X0));
#endif

  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

int uart_putchar(char c, FILE *stream) {
  if (c == '\n') {
    uart_putchar('\r', stream);
  }
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
  return 0;
}
int uart_getchar(FILE *stream) {
  loop_until_bit_is_set(UCSR0A, RXC0);
  return UDR0;
}

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void shift_out(uint8_t indata) {
  for (uint8_t i = 0; i < 8; i++) {
    /* Write bit to data port. */
    if (0 == (indata & _BV(7 - i))) {
      // digital_write(SHIFT_OUT_DATA, LOW);
      PORTB &= (0 << DATA);
    } else {
      // digital_write(SHIFT_OUT_DATA, HIGH);
      PORTB |= (1 << DATA);
    }

    /* Pulse clock to write next bit. */
    PORTD |= (1 << CLOCK);
    PORTD &= (0 << CLOCK);
  }
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder,
              uint8_t val) {
  uint8_t i;

  for (i = 0; i < 8; i++) {
    if (bitOrder == LSBFIRST)
      dataPin |= !!(val & (1 << i));
    else
      dataPin |= !!(val & (1 << (7 - i)));

    PORTD |= (1 << CLOCK);
    PORTD &= (0 << CLOCK);
  }
}
void shift_out_latch(void) {
  PORTD &= (0 << LATCH);
  PORTD |= (1 << LATCH);
}

int main(void) {

  shift_out_init();
  uart_init();
  stdout = &uart_output;
  stdin = &uart_input;
  char binary[17];
  while (1) {
    /* Show pattern for 5 seconds. */
    PORTD &= (0 << LATCH);
    shift_out(0xF9);
    shift_out(0xF1);
    PORTD |= (1 << LATCH);
    _delay_ms(5000);
    PORTD &= (0 << LATCH);
    shift_out(0x82);
    shift_out(0xF2);
    PORTD |= (1 << LATCH);
    _delay_ms(5000);
    PORTD &= (0 << LATCH);
    shift_out(0xB0);
    shift_out(0xF4);
    PORTD |= (1 << LATCH);
    _delay_ms(5000);
    PORTD &= (0 << LATCH);
    shift_out(0X90);
    shift_out(0xF8);
    PORTD |= (1 << LATCH);
    _delay_ms(5000);

    for (uint8_t i = 0; i < 10; i++) {
      PORTD &= (0 << LATCH);
      shift_out(SEGMENT_MAP[i]);
      shift_out(0xF8);
      PORTD |= (1 << LATCH);
      _delay_ms(1000);
    }
  }

  return 0;
}
