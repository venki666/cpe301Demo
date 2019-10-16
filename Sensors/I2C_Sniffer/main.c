/*
 * I2C_Sniffer.c
 *
 * Created: 8/21/2019 3:30:40 PM
 * Author : VenkatesanMuthukumar
 */ 

/** i2c sniffer using the TWI module on the atmega.
    This works by ACK-ing any reads/writes to any address and then quickly
    disabling the TWI module and using the SPI module on the SDA/SCL pins.
    Watch out that the slave that we're trying to sniff will always ACK its
    read + write commands.  We could get around this by putting a diode on
    the SDA line so the AVR can't pull the rest of the bus low and ack for
    the slave that it's trying to sniff. For example (Rp = pullup resistor):
        
             ||Rp    ||Rp
        SDA -----|<----- AVR
    
    Hardware Setup:
    SDA -> PC4 and PB5 (MOSI)
    SCL -> PC5 and PB7 (SCK)
    PB4 (SS) -> PB3 (to control SS)
    PB2 (INT2) -> SDA (for reading stop conditions)
    
    Software Setup:
    Read the Serial port at 115200 baud.  On startup, the AVR should say 'hi'
    After the buffer is full, the atmega will dump all the data read and reset
    the buffer.
    
    Alex Leone (acleone ~at~ gmail.com) 2009-02-25
*/
#define F_CPU 16000000UL									/* Define CPU clock Frequency e.g. here its 8MHz */

#include <avr/io.h>
#include <util/twi.h>

#define F_SCL   400000L
#define SNIFF_ADDR  0x29
#define BUFFER_LENGTH   900

#define SS_CONTROL     PB3
#define TWI_SCL        PC5
#define TWI_SDA        PC4
#define SERIAL_BAUD 9600UL

#define TW_ST_CHECK  (TWSR & TW_STATUS_MASK)
#define UBBRn_V     ((F_CPU / 16 + SERIAL_BAUD / 2) / SERIAL_BAUD - 1)
#define STATE_START    1
#define STATE_DATA_REC 2
#define STATE_STOP     4

#define _BV(bit) (1 << (bit))

struct info {
    uint8_t data;
    uint8_t state;
} buffer[BUFFER_LENGTH];
struct info *bp;

static void read_data(void);

static void add_to_buffer(uint8_t state, uint8_t data);
static void dump_buffer(void);

static void serial_init(void);
static void serial_printhex(uint8_t v);
static void serial_print(uint8_t byte);

void main(void)
{
    //PORTC = _BV(TWI_SCL) | _BV(TWI_SDA); // Enable pull-ups
    PORTC = 0xFF;
    DDRB |= _BV(SS_CONTROL);
    PORTB |= _BV(SS_CONTROL); // SS high until we start reading data
    
    serial_init();
    
    SPCR = _BV(SPE);   // enable SPI
    
    EICRA = _BV(ISC21) | _BV(ISC20); // rising edge
	
	bp = buffer;
	TWBR = (F_CPU/F_SCL - 16) / 2;
	TWAR = SNIFF_ADDR << 1;
	TWAMR = 0x7f << 1;
	TWCR = _BV(TWEA) | _BV(TWEN);
	
    for (;;) {
        if (TWCR & _BV(TWINT)) {
            uint8_t sr = TW_ST_CHECK;
            if (sr == TW_BUS_ERROR) { // clear any bus errors
                TWCR = _BV(TWINT) | _BV(TWSTO);
                TWCR = _BV(TWEA) | _BV(TWEN);
                continue;
            }
            add_to_buffer(STATE_START, TWDR);
            TWCR = _BV(TWINT); // disable TWI
            read_data();
            add_to_buffer(STATE_STOP, 0);
            TWCR = _BV(TWEA) | _BV(TWEN);
        }
	}	
}

/** Reads comms from this address. */
static void read_data(void)
{
    for (;;) {
        while (PINC & _BV(TWI_SCL))
            ; // wait for scl to drop low
        EIFR = _BV(INTF2); // clear INT2 flag
        PORTB &= ~_BV(SS_CONTROL); // enable SPI rec
        while (!(SPSR & _BV(SPIF))) {
            if (PINC & _BV(TWI_SCL)) {
                if (EIFR) {
                    // stop condition
                    // SDA goes high while SCL high
                    PORTB |= _BV(SS_CONTROL); // disable SPI
                    return;
                }
            } else {
                EIFR = _BV(INTF2);
            }
        }
        uint8_t byte_rec = SPDR;
        PORTB |= _BV(SS_CONTROL);
        while (PINC & _BV(TWI_SCL))
            ; // wait for scl to drop low
        while (!(PINC & _BV(TWI_SCL)))
            ; // wait for scl to pulse for ACK
        uint8_t ack = !(PINC & _BV(TWI_SDA));
        add_to_buffer(STATE_DATA_REC | ack, byte_rec);
    }
}

/** Adds state and data to the buffer. */
static void add_to_buffer(uint8_t state, uint8_t data)
{
    if (bp == buffer + BUFFER_LENGTH) {
        dump_buffer();
        bp = buffer;
        return;
    }
    bp->data = data;
    bp->state = state;
    bp++;
}

/** Dumps the contents of the buffer. */
static void dump_buffer(void)
{
    for (bp = buffer; bp < buffer + BUFFER_LENGTH; bp++) {
        if (bp->state == STATE_START) {
            serial_print('0');
            serial_print('x');
            uint8_t addr = bp->data >> 1;
            serial_printhex(addr);
            serial_print(' ');
            serial_print((bp->data & 0x01)? 'r' : 'w');
            serial_print(':');
        } else if (bp->state == STATE_DATA_REC) {
            // data w/ NACK
            serial_printhex(bp->data);
            serial_print(' ');
            serial_print('n');
        } else if (bp->state == (STATE_DATA_REC | 0x01)) {
            // data w/ ACK
            serial_printhex(bp->data);
        } else if (bp->state == STATE_STOP) {
            serial_print('s');
            serial_print('t');
            serial_print('o');
            serial_print('p');
            serial_print('\n');
        }
        serial_print('\n');
    }
    serial_print('\n');
    serial_print('\n');
}

/** Initializes the serial port and prints "hi\n". */
static void serial_init(void)
{
	UBRR0H = UBBRn_V >> 8;
	UBRR0L = UBBRn_V;
	UCSR0B = _BV(TXEN0);
	serial_print('h');
	serial_print('i');
	serial_print('\n');
}

/** Prints a 8 bit hex number. */
static void serial_printhex(uint8_t v)
{
    uint8_t vn = v >> 4;
    if (vn > 9) {
        serial_print('a' + (vn - 10));
    } else {
        serial_print('0' + vn);
    }
    vn = v & 0x0F;
    if (vn > 9) {
        serial_print('a' + (vn - 10));
    } else {
        serial_print('0' + vn);
    }
}

/** Outputs a byte over the USART. */
static void
serial_print(uint8_t byte)
{
    while (!(UCSR0A & _BV(UDRE0)))
        ;
    UDR0 = byte;
}



