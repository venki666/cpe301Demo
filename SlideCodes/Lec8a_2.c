/*
 * UART_RX_String.c
 *
 * Created: 10/21/2019 12:04:08 PM
 * Author : VenkatesanMuthukumar
 */ 

/*
 * serial_rx_328.c
 *
 * Created: 2015.06.22. 20:39:20
 *  Author: Bence
 */

#define F_CPU 16000000
#define BAUD 9600
#define MYUBRR F_CPU/BAUD/16-1
#define rx_buffer_size 128

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

unsigned char rx_buffer[rx_buffer_size];
volatile unsigned char rx_read_pos=0;
volatile unsigned char available=0;

void initUSART();
char serialRead();

FILE uart_input = FDEV_SETUP_STREAM(NULL, serialRead, _FDEV_SETUP_READ);

int main(void)
{
    sei();
    initUSART();
    DDRB=(1<<PINB1);
    stdin  = &uart_input;
    char string[32];
    while(1)
    {
        gets(string);
        
                if(strcmp(string,"be")==0)
                {
                    PORTB|=(1<<PINB1);
                }
                
                else if(strcmp(string,"ki")==0)
                {
                    PORTB&=~(1<<PINB1);
                }
    }
}

void initUSART()
{
    UBRR0H=((MYUBRR)>>8);
    UBRR0L=MYUBRR;
    
    UCSR0B=(1<<RXEN0)|(1<<RXCIE0);
    UCSR0C=(1<<UCSZ00)|(1<<UCSZ01);
}

char serialRead()
{
    char c=0;
    while(available<=0);
        if(rx_read_pos>=available)
        {
            c=rx_buffer[rx_read_pos-available];
        }
        else
        {
            c=rx_buffer[rx_buffer_size+rx_read_pos-available];
        }            
        available--;    
        return c;
}

 

ISR(USART_RX_vect)
{
    rx_buffer[rx_read_pos]=UDR0;
    rx_read_pos++;
    if(rx_read_pos>=rx_buffer_size)
    {
        rx_read_pos=0;
    }
    available++;
}
