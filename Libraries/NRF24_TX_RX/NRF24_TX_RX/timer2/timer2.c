/*
 * timer2.c
 *
 *  Created on: Jul 25, 2015
 *      Author: mikedanylov
 */

#include "timer2.h"



void timer2_init(){
    // set up timer with prescaler = 1024
    TCCR2B |= (1 << CS22);
    TCCR2B |= (1 << CS21);
	TCCR2B |= (1 << CS20);
    // initialize counter
    TCNT2 = 0;

    timer2_sei();
}

void timer2_sei(){
    // enable overflow interrupt
    TIMSK2 |= (1 << TOIE2);
    // enable global interrupts
    sei();
}

void timer2_cli(){
    // disable overflow interrupt
    TIMSK2 |= (0 << TOIE2);
}
