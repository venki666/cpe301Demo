#include <u8g2.h>
#include <stdint.h>
#define MOSI_DDR DDRB
#define MOSI_PORT PORTB
#define MOSI_PIN PINB
#define MOSI_BIT 3
#define SCK_DDR DDRB
#define SCK_PORT PORTB
#define SCK_PIN PINB
#define SCK_BIT 5
uint8_t u8x8_byte_avr_hw_spi(u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_avr_delay(u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);