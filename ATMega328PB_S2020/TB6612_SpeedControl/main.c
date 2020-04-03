/*
 * TB6612_MotorSpeed.c
 *
 * Created: 3/25/2020 1:27:23 PM
 * Author : VenkatesanMuthukumar
 */

#define F_CPU 16000000UL /* Define CPU Frequency e.g. here its 8MHz */
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>

// capture Flag
volatile uint8_t Flag;
volatile uint8_t Direction = 0;
volatile uint32_t revTickAvg;

void ADC_Init() /* ADC Initialization function */
{
  DDRC = 0x00;   /* Make ADC port as input */
  ADCSRA = 0x87; /* Enable ADC, with freq/128  */
  ADMUX = 0x40;  /* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel) /* ADC Read function */
{
  ADMUX = 0x40 | (channel & 0x07); /* set input channel to read */
  ADCSRA |= (1 << ADSC);           /* Start ADC conversion */
  while (!(ADCSRA & (1 << ADIF)))
    ; /* Wait until end of conversion by polling ADC interrupt flag */
  ADCSRA |= (1 << ADIF); /* Clear interrupt flag */
  _delay_us(1);          /* Wait a little bit */
  return ADCW;           /* Return ADC word */
}

// INT0 interrupt
ISR(INT0_vect) {
  // Use for Motor direction one trigger for forward, another for reverse
}

// INT1 interrupt
ISR(INT1_vect) {
  // Use for Motor direction one trigger for stop and go
}

volatile uint32_t revTick; // Ticks per revolution
volatile uint32_t revCtr;  // Total elapsed revolutions
volatile uint16_t T1Ovs2;  // Overflows for small rotations

// Initialize timer
void InitTimer1(void) {
  // Set PB0 as input
  DDRB &= ~(1 << DDB0);
  PORTB |= (1 << DDB0);

  // Set Initial Timer value
  TCNT1 = 0;
  ////First capture on rising edge
  TCCR1A = 0;
  TCCR1B = (0 << ICNC1) | (1 << ICES1);
  TCCR1C = 0;
  // Interrupt setup
  // ICIE1: Input capture
  // TOIE1: Timer1 overflow
  TIFR1 = (1 << ICF1) | (1 << TOV1);    // clear pending
  TIMSK1 = (1 << ICIE1) | (1 << TOIE1); // and enable
}

void StartTimer1(void) {
  // Start timer without pre-scaler
  TCCR1B |= (1 << CS10);
  // Enable global interrupts
  sei();
}

volatile uint32_t tickv, ticks;
// capture ISR
ISR(TIMER1_CAPT_vect) {
    tickv = ICR1; // save duration of last revolution
    revTickAvg = (uint32_t)(tickv) + ((uint32_t)T1Ovs2 * 0x10000L);
  revCtr++;  // add to revolution count
  TCNT1 = 0; // restart timer for next revolution
  T1Ovs2 = 0;
}
// Overflow ISR
ISR(TIMER1_OVF_vect) {
  // increment overflow counter
  T1Ovs2++;
}

int main(void) {
  char outs[72];
  USART_Init(9600);
  USART_SendString("Connected!\r\n"); // we're alive!
  InitTimer1();
  StartTimer1();
  USART_SendString("TIMER1 ICP Running \r\n");
  /* set PD2 and PD3 as input */
  DDRD &= ~(1 << DDD2);                            /* Make INT0 pin as Input */
  DDRD &= ~(1 << DDD3);                            /* Make INT1 pin as Input */
  PORTD |= (1 << DDD2) | (1 << DDD3);              // turn On the Pull-up
  DDRD |= (1 << DDD6) | (1 << DDD4) | (1 << DDD5); /* Make OC0 pin as Output */
  // We are manually setting the direction
  PORTD |= (1 << DDD5);               // CW Direction Set
  PORTD &= ~(1 << DDD4);              // CW Direction Set
  EIMSK |= (1 << INT0) | (1 << INT1); /* enable INT0 and INT1 */
  MCUCR |= (1 << ISC01) | (1 << ISC11) |
           (1 << ISC10); /* INT0 - falling edge, INT1 - raising edge */
  sei();                 /* Enable Global Interrupt */
  // WE are not using the ADC for speed - just manually setting the value
  ADC_Init(); /* Initialize ADC */
  TCNT0 = 0;  /* Set timer0 count zero */
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
  TCCR0B |=
      (1 << CS00) | (1 << CS02); /* Set Fast PWM with Fosc/64 Timer0 clock */
  OCR0A = 30;
  /* ready start value */
  while (1) {
    // Convert ticks to RPM
       // send Speed value to LCD or USART
      USART_SendString("Tick;Period;Frequency  ");
      snprintf(outs, sizeof(outs), "%f ", (float)revTickAvg); // print it
      USART_SendString(outs);
      USART_SendString(" \r\n");
  }
}
