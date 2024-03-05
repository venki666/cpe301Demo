;
; DB_example.asm
;
; Created: 2/8/2024 8:32:10 AM
; Author : venki
;

.cseg
.org 0x00
	LDI	R16,0x00
	OUT	DDRB,R16	
	LDI	R16,0xFF
	OUT	DDRC,R16	
	
	LDI	ZH,HIGH(XSQR_TABLE<<1)	
L1:	LDI	ZL,LOW(XSQR_TABLE<<1)	
	IN	R16,PINB	
	ANDI	R16,0x0F	
	ADD	ZL,R16
	LPM	R18,Z		
	OUT	PORTC,R18
	RJMP L1

.ORG 0x200
XSQR_TABLE:
	.DB 0, 1, 4, 9, 16, 25, 36, 49, 64, 81