/*
 * USART.c
 *
 * Created: 2/2/2012 2:21:34 PM
 *  Author: Chris Williams
 */ 

#include<avr/io.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h> 
#include"USART.h"

void USART_Init( unsigned int ubrr)
{
/*Set baud rate */
	DDRD |= (1 << PORTD1) | (1 << PORTD0);
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;
    
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);      // Enable receiver and transmitter and interrupt receive
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);    // Set frame: 8data, 1 stp
}


void USART_Transmit( unsigned char data )
{
	
	/* Wait for empty transmit buffer */
    while ( !(UCSR0A & (1 << UDRE0)) ) // Wait until buffer is empty

	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}


unsigned char USART_Receive( void )
{
	return '\0';
}


void USART_Print(char * input, int n)
{
	
	for(int i = 0; i < n; ++i)
	{
		USART_Transmit(input[i]);
	}
	USART_Transmit(';');
	
}

int print(char *format, ...)
{
	static char buf[128];
	va_list va;
	int n;
	va_start(va, format);
	n = vsprintf(buf, format, va);
	USART_Print(buf, n);
	va_end(va);
	return n;
	
}