/*
 * avr-lis3dh.c
 *
 * Created: 2/25/2019 10:53:57 PM
 * Author : VenkatesanMuthukumar
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// ov528 sample
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define UART_BAUD 9600
#define UART_TIMEOUT 100

#define SENSOR1_CS PB1
#define SENSOR1_OFFSET (0x10)
#define SENSOR1_SIZE 0x40
#define SENSOR2_CS PB2
#define SENSOR2_OFFSET (0x10 + 0x40)
#define SENSOR2_SIZE 0x40

static void delay_ms(uint16_t w){
	while (w-->0) _delay_ms(1);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// SPI util

void SPI_init_master() {
	PORTB |= (1<<SENSOR1_CS) | (1<<SENSOR2_CS); // disable chips
	DDRB |= (1<<PB5) | (1<<PB3) | (1<<SENSOR1_CS) | (1<<SENSOR2_CS); // SCK, MOSI, SS
	DDRB &= ~((1<<PB4));  // MISO
	SPCR = (1<<SPE)|(1<<MSTR) | (1<<SPR1);
}

uint8_t lis3dh_read8(uint8_t addr, uint8_t cs) {
	PORTB &= ~(1<<cs);
	_delay_us(10);
	SPDR = addr | 0x80;
	while(!(SPSR & (1 << SPIF)));
	SPDR = 0;
	while(!(SPSR & (1 << SPIF)));
	PORTB |= (1<<cs);
	return SPDR;
}

void lis3dh_read(uint8_t *buf, uint8_t addr, uint8_t sz, uint8_t cs) {
	PORTB &= ~(1<<cs);
	_delay_us(10);

	SPDR = addr | 0xC0;
	while(!(SPSR & (1 << SPIF)));
	while(sz) {
		SPDR = 0;
		while(!(SPSR & (1 << SPIF)));
		*buf = SPDR;
		sz--;
		buf++;
	}

	PORTB |= (1<<cs);
}


void lis3dh_write(uint8_t *buf, uint8_t addr, uint8_t sz, uint8_t cs) {
	PORTB &= ~(1<<cs);
	_delay_us(10);

	SPDR = addr | 0x40;
	while(!(SPSR & (1 << SPIF)));
	while(sz) {
		SPDR = *buf;
		while(!(SPSR & (1 << SPIF)));
		*buf = SPDR;
		sz--;
		buf++;
	}

	PORTB |= (1<<cs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// UART util

// uart recv buf
#define RX_BUF_SIZE 128
volatile uint8_t rx_buf[RX_BUF_SIZE];
volatile uint8_t rx_buf_pos = 0;
volatile uint8_t rx_buf_posr = 0;

ISR(USART_RX_vect) {
	if(bit_is_clear(UCSR0A,FE0)){// if no error.
		rx_buf[rx_buf_pos] = UDR0;
		rx_buf_pos = (rx_buf_pos+1) & (RX_BUF_SIZE-1);
	}
}

uint8_t recv_byte(void) {
	// while (bit_is_clear(UCSR0A, RXC0)); // Wait for data to be received.
	// return UDR0;
	while (rx_buf_posr == rx_buf_pos);
	uint8_t d = rx_buf[rx_buf_posr];
	rx_buf_posr = (rx_buf_posr+1) & (RX_BUF_SIZE-1);
	return d;
}

static inline uint8_t recv_ready(void) {
	return rx_buf_pos - rx_buf_posr;
}
void clear_rx_buf(void) {
	rx_buf_pos = rx_buf_posr = 0;
}

static void send_byte(uint8_t d) {
	while (bit_is_clear(UCSR0A, UDRE0)); // wait to become writable.
	UDR0 = d;
}

void writeBytes(uint8_t buf[], uint8_t len) {
	for (; len; len--, buf++) send_byte(*buf);
}

void uart_puts(char buf[]) {
	for (; *buf; buf++) send_byte(*buf);
}

uint8_t readBytes(uint8_t buf[], uint8_t len, uint16_t timeout_ms) {
	uint8_t i;
	uint8_t subms = 0;
	for (i = 0; i < len; i++) {
		while (recv_ready() == 0) {
			_delay_us(10);
			if (++subms >= 100) {
				if (timeout_ms == 0) {
					return i;
				}
				subms = 0;
				timeout_ms--;
			}
		}
		buf[i] = recv_byte();
	}
	return i;
}

void init_uart() {
	UBRR0 = 0; // stop UART
	UCSR0A |= _BV(U2X0); // 2x
	UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0); // enable Tx Rx Rx_INT
	UCSR0C = (1 << UCSZ00) |  (1 << UCSZ01); // 8bit
	UBRR0 = F_CPU/8/UART_BAUD-1; // Baud rate
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
//

#define CMD_LOAD 1
#define CMD_SAVE 2
#define CMD_READ 3
#define CMD_WRITE 4

uint8_t device_id;

static uint8_t EEMEM eemem_devid = 0;
//static uint8_t PROGMEM pgm_version[] = {'A',1,10,10, 0,0,  'Z', 'Z', 0,0,0,1, 0,0,0,0 };

int main(void) {
	DDRC = 0x00;
	PORTC = 0x20; // pull up PC5
	DDRD = 0xe0; // LEDs
	PORTD = 0x01;

	init_uart();
	SPI_init_master();

	eeprom_busy_wait();
	device_id = eeprom_read_byte(&eemem_devid);
	if (device_id == 0xff) device_id = 0;

	sei();

	for (uint8_t count = 0;;count ++) {
		if (recv_ready()) {
			uint8_t len = recv_byte();
			if (len < 40) {
				uint8_t buf[40];
				len--;
				readBytes(buf, len, UART_TIMEOUT);
				if (buf[2] == device_id) {
					// [cmd, opt, id, addr, len]
					if (buf[0] == CMD_READ) {
						uint8_t addr = buf[3];
						len = buf[4];
						if (addr == 0) {
							buf[4] = device_id;
							} else if (addr >= SENSOR1_OFFSET && addr < SENSOR1_OFFSET + SENSOR1_SIZE) {
							lis3dh_read(&buf[4], addr - SENSOR1_OFFSET, len, SENSOR1_CS);
							} else if (addr >= SENSOR2_OFFSET && addr < SENSOR2_OFFSET + SENSOR2_SIZE) {
							lis3dh_read(&buf[4], addr - SENSOR2_OFFSET, len, SENSOR2_CS);
							} else if (addr >= 0xA2) {
							for (uint8_t i = 0; i < len; i++) {
								//buf[4+i] = pgm_read_byte(&pgm_version[i]);
							}
						}
						buf[0] = len + 5;
						buf[1] = 0x80 | CMD_READ;
						buf[2] = 0x00;
						buf[3] = device_id;
						buf[len + 4] = 0;
						for (uint8_t i = 0; i< len + 4; i++) {
							buf[len + 4] += buf[i];
						}
						writeBytes(buf, buf[0]);
						} else if (buf[0] == CMD_WRITE) {
						if (buf[len-2] == 1) { // TODO multi mode.
							uint8_t addr = buf[len-3];
							if (addr == 0) {
								device_id = buf[3];
								} else if (addr >= SENSOR1_OFFSET && addr < SENSOR1_OFFSET + SENSOR1_SIZE) {
								lis3dh_write(&buf[3], addr - SENSOR1_OFFSET, len - 6, SENSOR1_CS);
								} else if (addr >= SENSOR2_OFFSET && addr < SENSOR2_OFFSET + SENSOR2_SIZE) {
								lis3dh_write(&buf[3], addr - SENSOR2_OFFSET, len - 6, SENSOR2_CS);
							}
							buf[0] = 5;
							buf[1] = 0x80 | CMD_WRITE;
							buf[2] = 0x00;
							buf[3] = device_id;
							buf[4] = 5 + (0x80 | CMD_WRITE) + device_id; // sum
							writeBytes(buf, buf[0]);
						}
						} else if (buf[0] == CMD_LOAD) {
						buf[0] = 5;
						buf[1] = 0x80 | CMD_LOAD;
						buf[2] = 0x00;
						buf[3] = device_id;
						buf[4] = 5 + (0x80 | CMD_LOAD) + device_id; // sum
						writeBytes(buf, buf[0]);
						} else if (buf[0] == CMD_SAVE) {
						eeprom_busy_wait();
						eeprom_update_byte(&eemem_devid, device_id);
						buf[0] = 5;
						buf[1] = 0x80 | CMD_SAVE;
						buf[2] = 0x00;
						buf[3] = device_id;
						buf[4] = 5 + (0x80 | CMD_SAVE) + device_id; // sum
						writeBytes(buf, buf[0]);
					}
					} else {
					// delay_ms(4);
					// clear_rx_buf();
				}
				} else {
				// debug
				delay_ms(4);
				send_byte(2);
				send_byte(recv_byte());
				uart_puts(".");
				clear_rx_buf();
			}
			} else {
			delay_ms(1);
		}

		if (count == 0) {
			if ((PINC & 0x20) == 0) {
				send_byte(lis3dh_read8(0x0f, SENSOR2_CS));
				uart_puts("Hello!\r\n");

				PORTD |= 0x80; // LED1
				delay_ms(200);
				PORTD &= ~0x80; // LED1
			}
			PORTD ^= 0x80; // LED1
		}
	}
	return 0;
}
