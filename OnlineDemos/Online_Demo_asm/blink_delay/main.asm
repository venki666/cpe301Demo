;
; blink_delay.asm
;
; Created: 8/21/2019 11:08:57 AM
; Author : VenkatesanMuthukumar
;

.org 0x00					;set program origin in memory
	LDI R16, 0XFF			;r16 = 0xff
	OUT DDRB, R16			;portb set as output
	LDI R16, 0b00111100		;r16 = 0x3c
	OUT PORTB, R16			;clear leds connected to portb
START:
	CBI PORTB, 0X02			;clear portb.2
	LDI R16, 41				;r16 = 41 for delay 0.5 sec running @ 16M
	CALL DELAY_X			;call delay subroutine
	SBI PORTB, 0X02			;set portb.2
	LDI R16, 41				;r16 = 41 for delay 0.5 sec running @ 16M
	CALL DELAY_X			;call delay subroutine
	RJMP START				;repeat from start

DELAY_X:
;Delay of exact Xs
    ldi  r19, 150
    ldi  r20, 128
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r16
    brne L1	
	RET				;return to main