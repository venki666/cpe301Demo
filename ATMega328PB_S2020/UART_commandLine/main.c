#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define BAUDRATE 57600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

char buffer[5];                //Output of the itoa function
uint16_t i=0;                //Variable for the for() loop

void USART_init(void);            //Function to initialize and configure the USART/serial
void USART_send( unsigned char data);    //Function that sends a char over the serial port
void USART_putstring(char* StringPtr);    //Function that sends a string over the serial port
unsigned char USART_Receive(void);
void UART_getLine(char* buf, uint8_t n);

int main(void){
	USART_init();        //Setup the USART
	
	for(;;){        //Our infinite loop
		UART_getLine(buffer, 5);
		i = atoi(buffer);
		itoa(i+100, buffer, 10);  
		USART_putstring(buffer);        //Send the converted value to the terminal
		USART_putstring("\n");            //Some more formatting
		_delay_ms(200);                //You can tweak this value to have slower or faster readings or for max speed remove this line
	}
	return 0;
}

void USART_init(void){
	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}
	
}

unsigned char USART_Receive(void)
{
	//Wait for data to be received
	while (!(UCSR0A & (1<<RXC0)));
	//Get and return received data from buffer
	return UDR0;
}

void UART_getLine(char* buf, uint8_t n)
{
	uint8_t bufIdx = 0;
	char c;

	// while received character is not carriage return
	// and end of buffer has not been reached
	do
	{
		// receive character
		c = USART_Receive();

		// store character in buffer
		buf[bufIdx++] = c;
	}
	while((bufIdx < n) && (c != '\r'));

	// ensure buffer is null terminated
	buf[bufIdx] = 0;
}