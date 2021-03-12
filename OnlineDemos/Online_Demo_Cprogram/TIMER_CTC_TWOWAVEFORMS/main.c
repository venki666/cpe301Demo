/*
 * TIMER_CTC_TWOWAVEFORMS.c
 *
 * Created: 3/11/2021 3:11:50 PM
 * Author : VenkatesanMuthukumar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
//Init PD5 and PD6 pins as output
void InitPort(void)
{//initial value 0
	PORTD &= ~((1<<5));
	//set PD6 and PD5 as output; but only PD5 used!?
	DDRD |= (1<<5);
}
//Initialize Timer0
void InitTimer0(void)
{
	//Set Initial Timer value
	TCNT0=0;
	//Place TOP timer values to Output compare registers
	OCR0A=255;
	OCR0B=75;
	//Set CTC mode
	//and disconnect PD6/OC0A and PD5/OC0B pins on compare match
	TCCR0A|= (1<<WGM01);
	//Enable Timer0 OCF0B Interrupt; Note OCR0A is the TOP for compare
	TIMSK0|=(1<<OCIE0A) | (1<<OCIE0B);
}
void StartTimer0(void)
{
	//Set prescaller 64 and start timer
	TCCR0B|=(1<<CS01)|(1<<CS00);
	//enable global interrupts
	sei();
	PORTD |= (1<<5);
}

ISR(TIMER0_COMPB_vect)
{
	PORTD &= (0<<5);
}

ISR(TIMER0_COMPA_vect)
{
 PORTD |= (1<<5);
 TCNT0 = 0;
}

int main(void)
{
	InitPort();
	InitTimer0();
	StartTimer0();
	while(1)
	{
		//doing nothing
	}
}
