;
; Timer1_Poll.asm
;
; Created: 2/13/2019 11:04:57 PM
; Author : VenkatesanMuthukumar
;

	LDI	R16,0x20
	SBI	DDRB,5	;PB5 as an output
	LDI	R17,0		
	OUT	PORTB,R17	
BEGIN:	LDI	R20,0xF2
	OUT	TCNT0,R20	;load timer0	
	LDI	R20,0x00
	OUT	TCCR0A,R20 ;Timer0,Normal mode, int clk
	LDI	R20,(1<<CS02| 1<<CS00)
	OUT	TCCR0B, R20; pre-scalar=CLK/1024
AGAIN:	IN	R20,TIFR0  ;read TIFR
	SBRS	R20,0 ;if TOV0 is set skip next inst.
	RJMP	AGAIN
	LDI	R20,0x0
	OUT	TCCR0B,R20	;stop Timer0
	LDI	R20,(1<<TOV0)	;R20 = 0x01
	OUT	TIFR0,R20	;clear TOV0 flag 
	EOR	R17,R16		;toggle D5 of R17
	OUT	PORTB,R17	;toggle PB5
	RJMP 	BEGIN