;
; Simple_8bitMultiplication.asm
;
; Created: 2/2/2021 1:57:20 AM
; Author : VenkatesanMuthukumar
;

.def	temp	=r25		;temporary storage variable
.EQU multiplicand = 0x0345		; multiplicand variable declaration
.EQU multiplier = 0x21			; multiplier variabledeclaration                                    
;***** Code
Start:
;---------------------------------------------------------------
;Include these lines for devices with SRAM (call MUL16_8)
	ldi	temp,low(RAMEND)
	out	SPL,temp	
	ldi	temp,high(RAMEND)
	out	SPH,temp	;init Stack Pointer
;---------------------------------------------------------------

initialize:						; INITIALIZING THE MULTIPLICAND AND MULTIPLIER
 ldi r24, low(multiplicand)		; loading in low value multiplicand
 ldi r25, high(multiplicand)	; loading in high value multiplicand
 ldi r23, 0						; 0 for comparison when decrementing multiplier
 ldi r22, multiplier			; loading in multiplier

loop:							; LOOP TO "MULTIPLY"
 add r18, r24					; add the low value of the multiplicand
 adc r19, r25					; add the high value of the multiplicand and the carry
 adc r20, r23					; add the carry into the new 3rd register
 dec r22						; decrement the multiplier until it reaches 0
 cp  r22, r23					; compare the multiplier with 0
 brne loop						; if the multiplier reaches 0, the program is finished
done:jmp done					; left 'jmp done' for debugging purposes                  