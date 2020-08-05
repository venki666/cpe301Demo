// Created by Davide Cavaliere
// E-mail: dadez87@gmail.com
// Website: www.monocilindro.com
// 27 September 2016

#ifndef SWSERIALE_cpp
#define SWSERIALE_cpp

#include "SWseriale.h"



// GLOBAL VARIABLES

// Data receive buffers
volatile uint8_t SWseriale_recv_buffer[SWSERIALE_RECV_BUF_SIZE];
volatile uint8_t SWseriale_recv_buffer_last_log_item = 0; // last logged item (from Timer2 interrupt)
volatile uint8_t SWseriale_recv_buffer_last_pro_item = 0; // last processed item (from program Main)
volatile uint8_t recv_bit_num = 0; // number of bits received, in current receiving byte
volatile uint8_t recv_byte_buffer; // content of the present byte being received (8 bits)

// Data sending buffers
volatile uint8_t SWseriale_send_buffer[SWSERIALE_SEND_BUF_SIZE];
volatile uint8_t SWseriale_send_buffer_last_added = 0;
volatile uint8_t SWseriale_send_buffer_to_send_now = 0;
volatile uint8_t send_bit_num = 0; // number of bits sent
volatile uint8_t send_byte_buffer; // content of the present byte being sent

volatile SWseriale_mode_enum SWseriale_mode = IDLE_MODE;


// FUNCTIONS

bool SWseriale_begin(){

	SWseriale_mode = IDLE_MODE; // Communication is set on idling

	// RX and TX pins settings
	DDRD &= ~(1 << RX_PIN); // Port D pin 3 set as Input (INT1)
	DDRD |= (1 << TX_PIN); // Port D pin 4 set as Output
	PORTD |= (1<< RX_PIN); // Pullup resistor activated on Port D pin 3
	PORTD |= (1<< TX_PIN); // Port D pin 4 set to "idle" (5V)

	// Timer 2 settings
	TCCR2A = _BV(WGM21); // CTC mode (WGM21 = 1, WGM20 = 0)
	TCCR2B = 0; // timer stopped (no prescaler is set)
	TCNT2 = 0; // counter is set to 0
	TIMSK2 = _BV(OCIE2A); // sets the timer overflow interrupt enable bit on compare

	// RX pin interrupt settings
	EICRA &= ~(_BV(ISC10)); // External Interrupt 1 falling edge (ISC10 = 0)
	EICRA |= _BV(ISC11); // External Interrupt 1 falling edge (ISC11 = 1)
	EIFR |=  _BV(INTF1); // Clears interrupt 1 request (just to make sure)
	EIMSK |= _BV(INT1); // Enable Interrupt 1 (waiting for message)

}

// Checks if there is any received data available (returns the number of available bytes, which are the one not already processed by Main program)
uint8_t SWseriale_available(){

  uint8_t temp_last_log_item = SWseriale_recv_buffer_last_log_item; // for buffering, to prevent changes from interrupt
  uint8_t temp_last_pro_item = SWseriale_recv_buffer_last_pro_item; // for buffering, to prevent changes from interrupt
  if (temp_last_log_item >= temp_last_pro_item){ // before buffer index rollover
    return (temp_last_log_item - temp_last_pro_item);
  }
  else{ // after buffer index rollover
    return (SWSERIALE_RECV_BUF_SIZE - temp_last_pro_item + temp_last_log_item);
  }

}

// Returns 1 byte read (the first byte acquired which has not been processed yet by Main program)
uint8_t SWseriale_read(){
  
  SWseriale_recv_buffer_last_pro_item++;
  if (SWseriale_recv_buffer_last_pro_item == SWSERIALE_RECV_BUF_SIZE) SWseriale_recv_buffer_last_pro_item=0; // rollover
  return SWseriale_recv_buffer[SWseriale_recv_buffer_last_pro_item];
  
}

// Sends the "data_array" of size "data_size" on SWseriale TX pin
bool SWseriale_write(uint8_t* data_array, uint8_t data_size){

  // Entry conditions
  if ((data_size == 0) || (data_size > SWSERIALE_SEND_BUF_SIZE)) return false; // size to send is not acceptable
  
  // Copies the data array in the sending buffer, so that the Main program can modify the array
  for (uint8_t i=0; i<data_size; i++){
    SWseriale_send_buffer[SWseriale_send_buffer_last_added] = data_array[i];
	SWseriale_send_buffer_last_added++;
	if (SWseriale_send_buffer_last_added == SWSERIALE_SEND_BUF_SIZE) SWseriale_send_buffer_last_added = 0; // rollover
  }

  return SWseriale_prepareToSend(); // This function returns "true" in case it was possible to initialize the message sending, "false" in case it was not possible (example: bus is already in SEND, or it is in RECV state)
}

// Prepares the serial bus for receiving (IDLE status, 5V). Timer2 is set
void SWseriale_listen(SWseriale_mode_enum listening_mode){

	// Only IDLE_MODE or RECV_MODE are accepted
	if ((listening_mode != IDLE_MODE) && (listening_mode != RECV_MODE)) listening_mode = IDLE_MODE; // Standard value (not possible to have, for example, SEND_MODE)

	if (listening_mode == IDLE_MODE) { // In case of IDLE_MODE, the counter has to stop (no need to send). In case of SEND_MODE, the counter has to continue for some bits
		TCCR2B = 0; // Timer 2 stopped
		EIFR |=  _BV(INTF1); // Clears interrupt 1 request (just to make sure)
	}

	SWseriale_mode = listening_mode; // ready for next task (IDLE_MODE or RECV_MODE)
	EIMSK |= _BV(INT1); // Enable Interrupt  again (wait for next byte from SWseriale)

}

// This function checks, at first, if any byte is requested to be sent.
// In positive case, it loads the byte to be sent, then remove the INT pin interrupt, set the timer, and finally set the bit to 0 (Start bit)
bool SWseriale_prepareToSend(){
	
	// Entry conditions
	if (SWseriale_send_buffer_to_send_now == SWseriale_send_buffer_last_added) return false; // no bytes required to be sent
	if ((!FORCE_SEND) && (SWseriale_mode != IDLE_MODE)) return false; // Serial bus is not in IDLE condition (it is either in SEND or RECV), so it is not possible to send due to busy condition

	// Initial setup for status and byte to be sent
	EIMSK &= ~(_BV(INT1)); // disables INT1 interrupt (no reception allowed when sending message - half duplex)
	SWseriale_mode = SEND_MODE; // status set to SEND
	send_byte_buffer = SWseriale_send_buffer[SWseriale_send_buffer_to_send_now]; // sets the byte to be sent
	SWseriale_send_buffer_to_send_now++; // current index increase, one more character will be sent soon
	if (SWseriale_send_buffer_to_send_now == SWSERIALE_SEND_BUF_SIZE) SWseriale_send_buffer_to_send_now = 0; // rollover
	send_bit_num = 0; // Start from bit 0

	// Setting Timer 2 to create an interrupt once every 1 bit (needed for sending each of the 10 bits)
	TCCR2B &= ~(_BV(CS22) | _BV(CS21) | _BV(CS20)); // Disables the timer counting (just to make sure)
	TCNT2 = 0; // counter is set to 0
	OCR2A = ONE_BIT_CYLES; // 1 bit time	// counter TOP value
	TCCR2B = _BV(CS21);  // Starts the timer // CTC (GWM22 = 0) Prescaler 1/8, 0.5us (CS22 = 0, CS21 = 1, CS20 = 0)

	// Port D pin 4 (TX) set to LOW (0) -> Start condition (5V -> 0V)
	PORTD &= ~(1 << TX_PIN); // Sets Port D bit 4 (TX) to 0 (Start condition)

	return true; // preparation finished correctly
}


// INTERRUPTS MANAGEMENT

// Called when a start bit is detected (5V -> 0V), which means that a byte is about to be received
ISR(INT1_vect) {
	if (SWseriale_mode != SEND_MODE){ // state is IDLE or RECV (i.e. not sending)
		
		// Disable INT1 interrupt
		EIMSK &= ~(_BV(INT1)); // disables INT1 interrupt (not needed anymore until this byte reading is finished)
		
		SWseriale_mode = RECV_MODE; // receiving byte
		recv_bit_num=0; // received bits counter reset to 0

		// Configures Timer 2 for start bit sampling
		TCCR2B &= ~(_BV(CS22) | _BV(CS21) | _BV(CS20)); // Disables the timer counting (just to make sure)
		TCNT2 = 0; // counter is set to 0
		OCR2A = SAMPLING_DELAY_CYLES; // about 0.5 bit time, in order to sample in the middle of the bit, to avoid noise	// counter TOP value
		TCCR2B = _BV(CS21);  // Starts the timer // CTC (GWM22 = 0) Prescaler 1/8, 0.5us (CS22 = 0, CS21 = 1, CS20 = 0)
	}
}

// This timer is activated after INT1 (start bit, 5V -> 0V) interrupt happens, or when sending byte is requested
ISR(TIMER2_COMPA_vect) {

	// Byte receiving case
	if (SWseriale_mode == RECV_MODE){
		if (recv_bit_num == 0) { // At the moment I am in the middle of start bit (0), need to sample in the middle of bit 1 next (distance is 1 bit)
			if (PIND & _BV(RX_PIN)){ // Need to make sure that the bit is 0 (=0V)
				SWseriale_listen(IDLE_MODE); // re-initialize receiving, in case external input was a mistake (this bit should be =0V because it is Start Bit)
				return;
			}
			OCR2A = ONE_BIT_CYLES; // 1 bit time (needed to jump from one bit to the next)
		}
		else if ((recv_bit_num >= 1) && (recv_bit_num < 9)){ // Data bits (1-8)
			if (PIND & _BV(RX_PIN)){ // RX pin bit = 1
				recv_byte_buffer |= 1 << (recv_bit_num - 1); // stores the value of the bit read, inside the buffer
			}else{ // bit = 0
				recv_byte_buffer &= ~(1 << (recv_bit_num - 1)); // stores the value of the bit read, inside the buffer
			}
		}
		else if (recv_bit_num == 9){ // when all 10 bits (0-9) have been received (all byte is completed), byte has to be added to the buffer
			SWseriale_recv_buffer_last_log_item++; // increase the buffer item counter
			if (SWseriale_recv_buffer_last_log_item == SWSERIALE_RECV_BUF_SIZE) SWseriale_recv_buffer_last_log_item=0; // rollover
			SWseriale_recv_buffer[SWseriale_recv_buffer_last_log_item]=recv_byte_buffer; // adds the single received byte to the received data buffer
			SWseriale_listen(RECV_MODE); // return in idle condition, keep the bus in "RECV" state a bit more (to allow the next byte to be received, also), so that, in case "FORCE_SEND=0" is defined, the send operation will be skipped
		}
		else if (recv_bit_num == 9+BITS_WAITING_AFTER_RECV){ // the bus is kept in "RECV" state for a little bit more, to allow the next byte to arrive
			SWseriale_mode = IDLE_MODE; // Set the bus in IDLE mode (so that, in case FORCE_SEND=0, there is no trouble, the check passes)
			if (!SWseriale_prepareToSend()) SWseriale_listen(IDLE_MODE); // Checks if any byte has to be sent (this is necessary in case any byte to be sent is pending the end of reception state). If not, timer is stopped.
			return;
		}
		recv_bit_num++;
	}
	
	// Byte sending case
	else if (SWseriale_mode == SEND_MODE){
		if ((send_bit_num >= 0) && (send_bit_num < 8)){ // Bits 0-7 (Data)
			if (send_byte_buffer & (1 << send_bit_num)){ // bit = 1
				PORTD |= (1 << TX_PIN); // Sets Port D bit 4 (TX) to 1 (Data)
			}else{ // bit = 0
				PORTD &= ~(1 << TX_PIN); // Sets Port D bit 4 (TX) to 0 (Data)
			}
		}
		else if (send_bit_num == 8){ // Stop bit
			PORTD |= (1 << TX_PIN); // Sets Port D bit 4 (TX) to 1 (Stop condition)
		}
		else if (send_bit_num == 10){ // Finished transmitting the byte (after Stop Bit, there is an additional bit with 5V status)
			SWseriale_listen(IDLE_MODE); // Set the bus in IDLE mode
			SWseriale_prepareToSend(); // Checks if any other byte needs to be sent
			return;
		}
		send_bit_num++;
	}
	
}

#endif
