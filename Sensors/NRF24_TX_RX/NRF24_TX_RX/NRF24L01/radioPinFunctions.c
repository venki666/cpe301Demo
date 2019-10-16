/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* Please define your platform spesific functions in this file ...
* -----------------------------------------------------------------------------
*/

#include <avr/io.h>

#define set_bit(reg,bit) reg |= (1<<bit)
#define clr_bit(reg,bit) reg &= ~(1<<bit)
#define check_bit(reg,bit) (reg&(1<<bit))

// Hardware ports and pins definitions
// change it for different AVRs
#define CE_PORT 	PORTC
#define CSN_PORT 	PORTC
#define SCK_PORT 	PORTB
#define MOSI_PORT 	PORTB
#define MISO_PORT 	PORTB

#define CE_DDR 		DDRC
#define CSN_DDR 	DDRC
#define SCK_DDR 	DDRB
#define MOSI_DDR 	DDRB
#define MISO_DDR 	DDRB

#define CE_BIT 		5
#define CSN_BIT 	4
#define SCK_BIT 	5
#define MOSI_BIT 	3
#define MISO_BIT 	4

#define MISO_PIN	PINB

/* ------------------------------------------------------------------------- */
void nrf24_setupPins(){
    set_bit(CE_DDR, CE_BIT); // CE output
    set_bit(CSN_DDR, CSN_BIT); // CSN output
    set_bit(SCK_DDR, SCK_BIT); // SCK output
    set_bit(MOSI_DDR, MOSI_BIT); // MOSI output
    clr_bit(MISO_DDR, MISO_BIT); // MISO input
}
/* ------------------------------------------------------------------------- */
void nrf24_ce_digitalWrite(uint8_t state){
    if(state){
        set_bit(CE_PORT, CE_BIT);
    }
    else{
        clr_bit(CE_PORT, CE_BIT);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_csn_digitalWrite(uint8_t state){
    if(state){
        set_bit(CSN_PORT, CSN_BIT);
    }
    else{
        clr_bit(CSN_PORT, CSN_BIT);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_sck_digitalWrite(uint8_t state){
    if(state){
        set_bit(SCK_PORT, SCK_BIT);
    }
    else{
        clr_bit(SCK_PORT, SCK_BIT);
    }
}
/* ------------------------------------------------------------------------- */
void nrf24_mosi_digitalWrite(uint8_t state){
    if(state){
        set_bit(MOSI_PORT, MOSI_BIT);
    }
    else{
        clr_bit(MOSI_PORT, MOSI_BIT);
    }
}
/* ------------------------------------------------------------------------- */
uint8_t nrf24_miso_digitalRead(){
    return check_bit(MISO_PIN, MISO_BIT);
}
