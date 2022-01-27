
 
.include "m328pdef.inc"                  ; Header file for the ATMega8515 
                                         ; microcontroller
 
                 LDI    R16,  0x00       ; Load 0b00000000 in R16
                 OUT    DDRB, R16        ; Configure PortB as an Input port
                 LDI    R16,  0xFF       ; Load 0b11111111 in R16
                 OUT    DDRC, R16        ; Configure PortC as an Output port
 
Again:           IN     R16, PINB        ; Read the values on the pins of PortB
                                         ; and store in R16
                 OUT    PORTC, R16       ; Write the value in R16 to the pins
                                         ; of PortC
                 RJMP   Again