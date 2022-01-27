;
; blink_button_delay_IRQ.asm
;
; Created: 7/24/2020 12:04:50 PM
; Author : VenkatesanMuthukumar
;


.ORG 0;location for reset
 JMP MAIN
.ORG 0x02;location for external interrupt 0
 JMP EX0_ISR
MAIN:
 LDI R20,HIGH(RAMEND)
 OUT SPH,R20
 LDI R20,LOW(RAMEND)
 OUT SPL,R20 ; initialize stack
 LDI R16, 0xFF

 LDI R20,0x2;make INT0 falling edge triggered
 STS EICRA,R20 
 SBI DDRB,2 ; PB5 is set to an output
 SBI PORTD,2 ; Sets pull-up
 LDI R20,1<<INT0 ; Enable INT0
 OUT EIMSK,R20 ; 
 SEI ; Enables interrupt
HERE:
 OUT PORTB, R16 
 RJMP HERE ; Keeps LED off
EX0_ISR:
 IN R21,PORTB
 LDI R22,(1<<2) ; 00100000 for toggling PB5
 EOR R21,R22 ; Toggles LED (PB2)
 OUT PORTB,R21 ; Turns LED (PB2) on
 RCALL DELAY ; Delays the LED to stay on
 RETI ; Returns to HERE until interrupt

DELAY: ; Delays the LED by roughly 1.25 seconds
 LDI R20, 125
D0:
 LDI R21, 125
D1:
 LDI R22, 255
D2:
 NOP
 NOP
 DEC R22
 BRNE D2
 DEC R21
 BRNE D1
 DEC R20
 BRNE D0
 RET