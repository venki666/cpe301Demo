;
; Online_Demo_asm.asm
;
; Created: 1/7/2019 3:58:19 PM
; Author : VenkatesanMuthukumar
;


.include <m328pbdef.inc>

.SET	COUNT = 0x25
.SET	COUNT1 = 0x19


.ORG 0x10
		LDI  R16, 0x25
.ORG 0x20
		LDI  R17, 0x00
		LDI  R18, 0x31
		LDI	R21, COUNT		;R21 = 0x25
		LDI	R22, COUNT + 3	;R22 = 0x28
		LDI	R21, COUNT1		;R21 = 0x19
		ADD R21, R22; r21= r21+r22
		ADC R21, R16; 
end:	jmp end