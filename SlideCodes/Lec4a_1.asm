;
; ASM_Pointers.asm
;
; Created: 2/4/2019 11:02:31 PM
; Author : VenkatesanMuthukumar
;

.EQU	STARTADDS	=	0x1330
.EQU	SNUMBER	=	0x5678

.ORG	0
	; Task #1
	; Initializes the pointer 0x0222 of register X
	LDI		R16, LOW(SNUMBER)
	MOV		R0, R16
	LDI		R16, HIGH(SNUMBER)
	LDI		ZL, LOW(STARTADDS<<1)
	LDI		ZH, HIGH(STARTADDS<<1)

	; Page Erase
	ldi spmcrval, (1<<PGERS) | (1<<SPMEN)
	call do_spm

	; re-enable the RWW section
	; must be avoided if the page buffer is pre-filled. Will flush the page buffer.
	ldi spmcrval, (1<<RWWSRE) | (1<<SPMEN)
	call do_spm

	LDI		ZL, LOW(STARTADDS<<1)
	LDI		ZH, HIGH(STARTADDS<<1)
	; execute Page Write
	ldi spmcrval, (1<<PGWRT) | (1<<SPMEN)
	call do_spm

	; re-enable the RWW section
	; must be avoided if the page buffer is pre-filled. Will flush the page buffer.
	ldi spmcrval, (1<<RWWSRE) | (1<<SPMEN)
	call do_spm

END:RJMP END

do_spm:
	;input: spmcrval determines SPM action
	;disable interrupts if enabled, store status

	;in r19, SREG
	;cli
	;check for previous SPM complete
	;wait: 
	;	in r18, SPMCSR
	;	sbrc r18, SPMEN
	;	rjmp wait
	;SPM timed sequence
	out SPMCSR, r16
	spm
	;restore SREG (to enable interrupts if originally enabled)
	;out SREG, r19
	;sei
ret
