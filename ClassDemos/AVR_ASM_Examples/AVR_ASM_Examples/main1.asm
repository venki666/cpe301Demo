.include <m328pdef.inc>
;This program gets data from channel 0 (ADC0) of 
;ADC and displays the result on Port C and Port D. This is done 
;forever. 
	LDI	R16,0xFF
	OUT	DDRB, R16		;make Port B an output
	OUT	DDRD, R16		;make Port D an output
	LDI	R16,0
	OUT	DDRC, R16		;make Port C an input for ADC
	LDI	R16,0x87		;enable ADC and select ck/128
	STS	ADCSRA, R16
	LDI	R16,0xC0		;1.1V Vref, ADC0 single ended
	STS	ADMUX, R16		;input, right-justified data
READ_ADC:
	LDI	R16,0x87|(1<<ADSC)
	STS	ADCSRA,R16		;start conversion
KEEP_POLLING:			;wait for end of conversion 
	LDS	R16,ADCSRA
	SBRS R16,ADIF		;is it end of conversion yet?
	RJMP KEEP_POLLING	;keep polling end of conversion	
	LDI	R16,(1<<ADIF)
	STS	ADCSRA,R16		;write 1 to clear ADIF flag
	LDS	R16,ADCL		;YOU HAVE TO READ ADCL FIRST
	OUT	PORTD,R16		;give the low byte to PORTD
	LDS	R16,ADCH		;READ ADCH AFTER ADCL 
	OUT	PORTB,R16		;give the high byte to PORTB
	RJMP	READ_ADC		;keep repeating it      

