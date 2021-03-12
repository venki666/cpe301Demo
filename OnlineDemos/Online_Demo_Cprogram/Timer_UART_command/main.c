/*
 * simple_UART_command.c
 *
 * Created: 10/10/2019 9:39:55 PM
 * Author : VenkatesanMuthukumar
 */ 

/*** MACROS *******************************************************************/

#define F_CPU 16000000UL			// required for setbaud & other libraries
#define BAUD 9600UL				// desired baud
#define BAUD_TOL 2					// desired baud rate tolerance (+/- %)

/*** INCLUDES *****************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>					// SFR/Bit identifiers
#include <avr/interrupt.h>			// ISR macro
#include <util/setbaud.h>			// Baud rate calculation macro helpers

/*** GLOBAL VARIABLES *********************************************************/

char g_OutString[80];				// output buffer used with sprintf
volatile uint8_t g_TickSignal = 0;	// define shared variable used for signaling
uint8_t g_Hours, g_Minutes, g_Seconds, g_Tenths; // time variables
enum TIMEOUTPUT {DISABLED, ENABLED} g_TimeControl;	// control for time output
char g_Command;						// control character to enable/disable output

/*** LOCAL FUNCTION PROTOTYPES ************************************************/

void SYSTEM_Init(void);				// Initialize HW and variables
void TIMER1_Init(void);				// Timer/Counter1 initialization
void USART0_Init(void);				// USART0 initialization
void USART0_Put(uint8_t data);		// Transmit a byte
void USART0_PutString(char *ptr);	// Transmit a string
char USART0_GetChar(void);			// Receive a character (if available)

/*** main() *******************************************************************/

int main(void)
{
	// Initialization
	SYSTEM_Init();
	
	// Enable global interrupts
	sei();
	
	USART0_PutString("megaAVR USART0 Example (Polled)\r\n\r\n");
	USART0_PutString("Press 'u' to update the clock display,\r\n");
	USART0_PutString("Press 'f' to freeze the clock display.\r\n\r\n");
	sprintf(g_OutString, "%02d:%02d:%02d\r", g_Hours, g_Minutes, g_Seconds);
	USART0_PutString(g_OutString);
	
	while(1){
		
		// Look for 100mS event signal
		if(g_TickSignal){
			
			// Toggle LED0
			PORTB ^= (1 << PORTB5);
			
			// Update the clock
			g_Tenths++;
			if(g_Tenths == 10){
				g_Tenths = 0;
				if(g_TimeControl == ENABLED){
					sprintf(g_OutString, "%02d:%02d:%02d\r", g_Hours, g_Minutes, g_Seconds);
					USART0_PutString(g_OutString);
				}
				g_Seconds++;
				if(g_Seconds == 60){
					g_Seconds = 0;
					g_Minutes++;
				}
				if(g_Minutes == 60){
					g_Minutes = 0;
					g_Hours++;
				}
			}
			
			// Acknowledge event signal
			g_TickSignal = 0;
		}
		
		// Look to enable/disable the updating of the clock output
		g_Command = USART0_GetChar();
		switch(g_Command){
			case 'u':
			g_TimeControl = ENABLED;
			break;
			
			case 'f':
			g_TimeControl = DISABLED;
			break;
			
			default:
			break;
		}
	}
} // main()

/*** SYSTEM_Init() ****************************************************************/

void SYSTEM_Init(void){
	
	// Set LED as output
	DDRB |= (1<<PORTB5);			// Configure PB5 as digital output
	PORTB &= ~(1<<PORTB5);			// Set initial level for PB5
	
	// Timer1
	TIMER1_Init();
	
	// USART0
	USART0_Init();
	
	// Initialize global variables
	g_Hours = 0;
	g_Minutes = 0;
	g_Seconds = 0;
	g_Tenths = 0;
	
} // SYSTEM_Init()

/*** TIMER1_Init() *************************************************************/

void TIMER1_Init(void){
	
	// Set up Timer/Counter1
	TCCR1B |= (1 << WGM12);			// Configure timer 1 for CTC mode
	OCR1A = 25000;					// Set CTC compare value to 10Hz (100mS) at
	// 16MHz AVR clock , with a prescaler of 64
	TIMSK1 |= (1 << OCIE1A);		// Enable CTC interrupt
	TCCR1B |= ((1 << CS10) | (1 << CS11));	// Start Timer/Counter1 at F_CPU/64

} // TIMER1_Init()

/*** USART0_Init() *************************************************************/

void USART0_Init(void){
	
	// Set the BAUD rate
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X					// USE_2X defined by setbaud.h based on inputs
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
	
	// Set the Mode & Frame Parameters
	// Asynchronous, 8-data, No parity, 1 stop
	UCSR0C = 0x06;
	
	// Enable USART0 Transmitter and Receiver
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

} // USART0_Init()

/*** USART0_Put() **************************************************************/

void USART0_Put(uint8_t data){
	
	//Checking to see if USART TX buffer is empty for new data
	while(!(UCSR0A & (1<<UDRE0)));
	
	//Initiating transfer
	UDR0 = data;

} // USART0_Put()

/*** USART0_PutString() ********************************************************/

void USART0_PutString(char *ptr){
	
	while(*ptr){			// Loop until end of string (*s = '\0')
		USART0_Put(*ptr++);	// Send the character and point to the next one
	}
	
} // USART0_PutString()

/*** USART0_GetChar() **************************************************************/

char USART0_GetChar(void){
	
	char rxdata;
	
	if(UCSR0A & (1<<RXC0)){		// checking if USART RX data is available
		rxdata = UDR0;			// reading the received byte (clears RXC0)
		return rxdata;			// return the data
	}
	
	return 0x00;				// return NUL char ('/0') if no data available

} // USART0_GetChar()

/*** Interrupt Handler for TIMER1_COMPA Interrupt (Nesting Disabled) **********/

ISR(TIMER1_COMPA_vect, ISR_BLOCK)
{
	g_TickSignal++;					// signal "tick" event
	// Timer1 CTC flag auto-cleared by hardware
}

