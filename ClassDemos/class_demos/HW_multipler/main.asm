;
; HW_multipler.asm
;
; Created: 9/5/2019 10:51:34 PM
; Author : VenkatesanMuthukumar
;


; Replace with your application code
start:
	LDI r17, 0x03
	LDI r16, 0x01
	LDI r24, 0x05
	LDI r23, 0x02
	call M_16x16
loop:
	jmp loop

; [ 16x16 Bit Unsigned Multiplication ]
; multiplicand: r17:r16
; multiplier  : r24:r23
; result out  : r21:r20:r19:r18
; 24 cycles
M_16x16:
    MUL   r17, r24              ; ah * bh
    MOVW  r21:r20, r1:r0
    MUL   r16, r23              ; al * bl
    MOVW  r19:r18, r1:r0
    MUL   r17, r23              ; ah * bl
    CLR   r23
    ADD   r19, r0
    ADC   r20, r1
    ADC   r21, r23
    MUL   r24, r16              ; bh * al
    ADD   r19, r0
    ADC   r20, r1
    ADC   r21, r23
    RET



;* From AVR Instruction Set Guide, pg 99-100.
;* Signed multiply of two 16-bit numbers with 32-bit result.
;* r19:r18:r17:r16 = r23:r22 * r21:r20
/*	muls16x16_32:
	clr r2
	muls r23, r21 ;(signed)ah * (signed)bh
	movw r19:r18, r1:r0
	mul r22, r20 ;al * bl
	movw r17:r16, r1:r0
	mulsu r23, r20 ;(signed)ah * bl
	sbc r19, r2
	add r17, r0
	adc r18, r1
	adc r19, r2
	mulsu r21, r22 ;(signed)bh * al
	sbc r19, r2
	add r17, r0
	adc r18, r1
	adc r19, r2
	ret*/