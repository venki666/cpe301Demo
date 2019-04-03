;
; Assignment1B.asm
;
; Created: 2/13/2019 10:33:10 PM
; Author : VenkatesanMuthukumar
;

RESET:
.equ STARTADDS = 0x0222 ; Starting Address
.equ count = 255 ; Part 1 of the counter to 300
.equ count1 = 46 ; Part 2 of the counter to 300

.org 0
	clr r0	; Clearing r0 register
	ldi XL, low(STARTADDS)	; XL = STARTADDS[7:0]
	ldi XH, high(STARTADDS) ; XH = STARTADDS[15:8]
	ldi YL, low(0x400)	; YL = 0x00
	ldi YH, high(0x400) ; YH = 0x04
	ldi ZL, low(0x600)	; ZL = 0x00
	ldi ZH, high(0x600)	; ZH = 0x06

	; Initializing count registers
	ldi r21, count	; r21 = 255
	ldi r22, count1 ; r22 = 46
	ldi r20, 5 ; r20 = 5

	; Clearing Registers for sums 
	clr r0	; R0 = 0
	clr r16	; R16 = 0
	clr r17	; R17 = 0
	clr r18	; R18 = 0
	clr r19	; R19 = 0
	clr r5 ; r5 = 0
	inc r5 ; r5 = 1

START:

; Adding lower and upper address bits to be stored
	mov r1, XL ; R1 = XL
	add r1, XH ; R1 = XL + XH
	mov r3, r1 ; R3 = R1
	st	X+, r3 ;	 [X] = XL + XH

	; Checking if divisble by 5 
DIVBYFIVE:
	cp	r1, r20	; if(R1 < 5)
	brlo DIVBAD	; go to DIVBAD
	sub r1, r20	; R1 = R1 - R20
	cp	r1, r0	; if(R1 == 0)
	breq DIVGOOD	;go to DIVGOOD
	rjmp	 DIVBYFIVE ; go to DIVBYFIVE

DIVGOOD:
	st Z+, r3	; [Z] = XL + XH
	add r16, r3	; R16 = R16 + R3 (lower)
	adc r17, r0	; R17 = R17 + R0 + C (upper)
	rjmp DONE	; go to DONE

DIVBAD:
	st Y+, r3	; [Y] = XL + XH
	add r18, r3	; R18 = R18 + R3 (lower)
	adc r19, r0	; R19 = R19 + R0 + C (upper)
	rjmp DONE	; goto DONE
	

DONE:
	cp r21, r0	; if(count == 0)
	breq NOTYET	; go to NOTYET
	sub r21, r5	; R21 = R21 = R5
	brne START	; if(R21 != 0) go to START

NOTYET:
	sub r22, r5	; R22 = R22 - R5 
	brne Start	; if(R22 != 0) go to START

FIN:
	rjmp RESET	; Reset

