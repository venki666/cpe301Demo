// Created by Davide Cavaliere
// E-mail: dadez87@gmail.com
// Website: www.monocilindro.com
// 27 September 2016

#ifndef SWSERIALE_h
#define SWSERIALE_h

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sfr_defs.h>
//#include <Arduino.h>

#define BAUDRATE 9600 // baudrate 9600 bit/s - do not change
#define ONE_BIT_CYLES (uint8_t)(2000000 / BAUDRATE) // -> pre-scaler set to 0.5us - Example: at 9600 baud, this is 208 (208.3333...)
#define SAMPLING_DELAY_CYLES (uint8_t)(ONE_BIT_CYLES / 2) // the bit sampling time should be in the middle of the bit, to avoid noise due to sampling during rising or falling time. Example: @16Mhz, prescaler = 8, one timer tick happens in 0.5us; 100 cycles means 50us after start of bit
#define RX_PIN 3 // RX pin (INT1) - do not change (if you plan to change this to INT0, pin2, you should also change the *.cpp file in order to set interrupts on pin2, INT0)
#define TX_PIN 4 // TX pin - can be changed freely, as long as it is a pin on Port D, and it is within 4-7 (notice that 0=RXD native, 1=TXD native, 2=INT0, 3=INT1). If you plan to use an other Port (B or C), you need to change *.cpp file in order to use a differnt port
#define SWSERIALE_RECV_BUF_SIZE (uint8_t)64 // buffer size for serial data recv
#define SWSERIALE_SEND_BUF_SIZE (uint8_t)32 // buffer size for serial data send
#define FORCE_SEND 0 // forces new sending even if Serial status is still receiving or sending previous message
#define BITS_WAITING_AFTER_RECV 5 // these are the bits which are waited, after the complete reception of a byte, before setting the bus again to IDLE (this is necessary to avoid sending a byte, interrupting receiving phase of 2 consecutive bytes. It has effect when FORCE_SEND=0)

#define bool uint8_t
#define true 1
#define false 0


typedef enum {
	IDLE_MODE = 0, // Waiting for start bit
	RECV_MODE, // Finish receiving byte
	SEND_MODE // Sending byte
}SWseriale_mode_enum;



    bool SWseriale_begin(); // First initialization
    void SWseriale_listen(SWseriale_mode_enum listening_mode); // Listens in IDLE or RECV modes
    uint8_t SWseriale_available();
    uint8_t SWseriale_read();
    bool SWseriale_prepareToSend();
    bool SWseriale_write(uint8_t* data_array, uint8_t data_size);


#endif
