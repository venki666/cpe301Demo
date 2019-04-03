;
; Online_Demo_asm.asm
;
; Created: 1/7/2019 3:58:19 PM
; Author : VenkatesanMuthukumar
;


.include <m328pdef.inc>

.SET	COUNT = 0x25
.SET	COUNT1 = 0x19


.ORG 0
		LDI  R16, 0x25
.ORG 0x7
		LDI  R17, 0x34
		LDI  R18, 0x31
		LDI	R21, COUNT		;R21 = 0x25
		LDI	R22, COUNT + 3	;R22 = 0x28
		LDI	R21, COUNT1		;R21 = 0x19
end:	jmp end