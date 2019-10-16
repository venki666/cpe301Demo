/*
 * timer2.h
 *
 *  Created on: Jul 25, 2015
 *      Author: mikedanylov
 */

#ifndef TIMER2_TIMER2_H_
#define TIMER2_TIMER2_H_

/*
 * Waiting time is calculated
 * considering Interrupt Frequency = 2.04 Hz if F_CPU==128000
 *
 * For example, if WAIT_TIME = 10
 * will generate ~20.4s of waiting time
 */
#include <avr/io.h>
#include <avr/interrupt.h>

extern void timer2_init();
extern void timer2_sei();
extern void timer2_cli();

#endif /* TIMER2_TIMER2_H_ */
