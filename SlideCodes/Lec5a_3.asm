
.include <m328pdef.inc>
 .ORG 0;location for reset
       JMP MAIN

MAIN:
       CBI  DDRC, 2	;make PC2 an input
       CBI  PORTC,2	; TURN-OFF pull-up 
       LDI  R16, 0x1E	
       OUT  DDRB, R16	;make Port B 5,4,3,2 an output pin
	   OUT  PORTB, R16 ; sel PORTB values - LED Off
       SBI  DDRD, 2	;make PD3 an output
AGAIN: SBIC PINC, 2	;Skip if Bit PB2 is HIGH
       RJMP AGAIN		;keep checking if LOW
       LDI  R16, 0x0E	
       OUT  PORTB, R16	;write 0x45 to port C
       SBI  PORTD, 2	;set bit PD3 (H-to-L)
	   RCALL delay_1ms 
       CBI  PORTD, 2	;clear bit PD3
	   RCALL delay_1ms
HERE:  RJMP HERE

delay_1ms:
	push r16 ;save the value in r16
	ldi r16,99 ;accounts for overhead of 12 cycles.
	delay_1ms1: ; 10 us/loop
	nop ; [1 cycle ]
	nop
	nop
	nop
	nop
	nop
	nop
	dec r16
	brne delay_1ms1 ; [2 cycles ]
	pop r16 ;restore the value in r16
	ret