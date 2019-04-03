;
; ASM_Pointers.asm
;
; Created: 2/4/2019 11:02:31 PM
; Author : VenkatesanMuthukumar
;

.EQU	STARTADDS	=	0x0222
.EQU	MEM_DIV_5	=	0x0400
.EQU	MEM_NDIV_5	=	0x0600

.ORG	0
	; Task #1
	; Initializes the pointer 0x0222 of register X
	LDI		XL, LOW(STARTADDS)
	LDI		XH, HIGH(STARTADDS)

	LDI		R16, 100				; Inner loop which counts to 100 (R16)
LP1:
	MOV		R0, XL					; Use R0 for LOW(STARTADDS)
	MOV		R1, XH					; Use R1 for HIGH(STARTADDS)
	ADD		R0, R1					; LOW(STARTADDS) + HIGH(STARTADDS)
	ST		X+, R0					; Stores the results
	DEC		R16						; Updates loop counter
	BRNE	LP1						; Checks loop condition (R16 != 0)
END:RJMP END
