/*
 * ATmega16_Interfacing_to_Nokia5110.c
 *
 * http://www.electronicwings.com
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "spi_master.h"
#include "font.h"

void N5110_Cmnd(char DATA)
{
	PORTB &= ~(1<<DC);				/* make DC pin to logic zero for command operation */
	SPI_SS_Enable();				/* enable SS pin to slave selection */	
	SPI_Write(DATA);				/* send data on data register */
	PORTB |= (1<<DC);				/* make DC pin to logic high for data operation */
	SPI_SS_Disable();
}

void N5110_Data(char *DATA)
{
	PORTB |= (1<<DC);									/* make DC pin to logic high for data operation */
	SPI_SS_Enable();									/* enable SS pin to slave selection */
	int lenan = strlen(DATA);							/* measure the length of data */
	for (int g=0; g<lenan; g++)
	{
		for (int index=0; index<5; index++)
		{
			SPI_Write(ASCII[DATA[g] - 0x20][index]);	/* send the data on data register */			
		}
		SPI_Write(0x00);
	}							
	SPI_SS_Disable();									
}

void N5110_Reset()					/* reset the Display at the beginning of initialization */
{
	PORTB &= ~(1<<RST);
	_delay_ms(100);
	PORTB |= (1<<RST);
}

void N5110_init()
{
	N5110_Reset();					/* reset the display */
	N5110_Cmnd(0x21);				/* command set in addition mode */
	N5110_Cmnd(0xC0);				/* set the voltage by sending C0 means VOP = 5V */
	N5110_Cmnd(0x07);				/* set the temp. coefficient to 3 */
	N5110_Cmnd(0x13);				/* set value of Voltage Bias System */
	N5110_Cmnd(0x20);				/* command set in basic mode */
	N5110_Cmnd(0x0C);				/* display result in normal mode */
}

void lcd_setXY(char x, char y)		/* set the column and row */
{
	N5110_Cmnd(x);
	N5110_Cmnd(y);
}

void N5110_clear()					/* clear the Display */
{
	SPI_SS_Enable();
	PORTB |= (1<<DC);
	for (int k=0; k<=503; k++)
	{
		SPI_Write(0x00);		
	}
	PORTB &= ~(1<<DC);
	SPI_SS_Disable();	
}

void N5110_image(const unsigned char *image_data)		/* clear the Display */
{
	SPI_SS_Enable();
	PORTB |= (1<<DC);
	for (int k=0; k<=503; k++)
	{
		SPI_Write(image_data[k]);
	}
	PORTB &= ~(1<<DC);
	SPI_SS_Disable();
}

int main()
{
	SPI_Init();
	N5110_init();
	N5110_clear();
	lcd_setXY(0x40,0x80);
	N5110_Data("ElectronicWings");
	while(1)	
	return 0;
}