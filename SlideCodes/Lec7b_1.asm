;
; Timer0_N_IRQ.asm
;
; Created: 2/13/2019 11:45:23 PM
; Author : VenkatesanMuthukumar
;

;Program 
.ORG	0x0	;location for reset 
	JMP	MAIN
.ORG	0x20	;loc. for Timer0 over.
	JMP	T0_OV_ISR
;----main program for initialization
.ORG	0x100
MAIN:	LDI	R20,HIGH(RAMEND)
	OUT	SPH,R20
	LDI	R20,LOW(RAMEND)
	OUT	SPL,R20	
	SBI	DDRB,5	;output
	LDI	R20,0
	OUT	DDRC, R20
	LDI	R20,0xFF
	OUT	DDRD, R20
	LDI	R20,(1<<TOIE0)	
	STS	TIMSK0,R20
	SEI	
	LDI	R20,-32  ;value for 4µs 
	OUT	TCNT0,R20
	LDI	R20,0x01
	OUT	TCCR0B,R20
HERE:	IN	R20,PINC	  
	OUT 	PORTD,R20   
	JMP	HERE	
;--------ISR for Timer 0	
T0_OV_ISR: ; 
	LDI	R21,(1<<TOV0)	;R20 = 0x01
	OUT	TIFR0,R21	;clear TOV0 flag 
	IN	R16,PORTB
	LDI	R17,0x20
	EOR	R16,R17
	OUT 	PORTB,R16
	RETI	
