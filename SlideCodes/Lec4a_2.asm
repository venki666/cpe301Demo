;
; ASM_pointer_division.asm
;
; Created: 2/4/2019 11:39:56 PM
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



	; Initializes the pointer 0x0222 of register X
	LDI		XL, LOW(STARTADDS)
	LDI		XH, HIGH(STARTADDS)
	; Initializes the pointer 0x0400 of register Y
	LDI		YL, LOW(MEM_DIV_5)
	LDI		YH, HIGH(MEM_DIV_5)
	; Initializes the pointer 0x0600 of register Z
	LDI		ZL,	LOW(MEM_NDIV_5)
	LDI		ZH,	HIGH(MEM_NDIV_5)

	LDI		R24, 0x00				; Count the number of numbers divisible by 5
	LDI		R25, 0x00				; Count the number of numbers isn't divisible by 5

	LDI		R16, 100				; loop which counts to 100 ($16)
LP3:
	LD		R18, X					; Loads the number to be modified (masking) into R18
	LD		R19, X+					; Saves the unmodified number into R19
SUB_LOOP:
	CPI		R18, 0x05				; Checks if the current result is divisible by 5
	BREQ	DIV_5
	SUBI	R18, 0x05				; Subtracts the number being tested by 5
	BRLO	NOT_DIV_5				; If an underflow occurs, the number is not divisible by 5
	RJMP	SUB_LOOP
DIV_5:
	ST		Y+, R19					; Stores the number that was divisible by 5
	INC		R24						; Updates count (Divisible)
	RJMP	END_DIV
NOT_DIV_5:
	ST		Z+,	R19					; Stores the number that was not divisible by 5
	INC		R25						; Updates count (Not Divisible)
END_DIV:
	DEC		R16						; Updates loop counter
	BRNE	LP3						; Checks loop condition (R16 != 0)

END:RJMP END