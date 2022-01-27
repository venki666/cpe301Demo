.include "m8515def.inc"
 
                ;Initialize the microcontroller stack pointer
                LDI    R16, low(RAMEND)
                OUT    SPL, R16
                LDI    R16, high(RAMEND)
                OUT    SPH, R16
 
                ;Configure portB as an output put
                LDI    R16, 0xFF
                OUT    DDRB, R16
 
                ;Toggle the pins of portB
                LDI    R16, 0xFF
                OUT    PORTB, R16
                RCALL  Delay
                LDI    R16, 0x00
                OUT    PORTB, R16
                RCALL  Delay
 
                ;Delay subroutine
Delay:          LDI  R17, 0xFF
loop:           DEC  R17
                BRNE loop
                RET