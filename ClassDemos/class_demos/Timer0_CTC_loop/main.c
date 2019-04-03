/*
 * Timer0_CTC_loop.c
 *
 * Created: 3/7/2019 8:50:36 PM
 * Author : VenkatesanMuthukumar
 */

#include <avr/io.h>

int main(void) {
  uint8_t OVFCount = 0;
  // set PD6 as output
  DDRB |= (1 << DDB2);
  // Clear the output bit -led on
  PORTB &= ~(1 << DDB2);
  // Set Initial Timer value
  TCNT0 = 0;
  // Place TOP timer value to Output compare register
  OCR0A = 0x9B;
  // Set CTC mode
  TCCR0A |= (1 << WGM01);
  // Set pre-scalar 1024 and start timer
  TCCR0B |= (1 << CS02) | (1 << CS00);
  while (1) {
    while ((TIFR0 & 0x02) == 0)
      ;
    TIFR0 = 0x02; // clear timer1 compare overflow flag
    OVFCount++;
    if (OVFCount == 10) {
      PORTB ^= (1 << DDB2);
      OVFCount = 0;
    }
  }
}
