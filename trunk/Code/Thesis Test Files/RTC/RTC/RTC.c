/*
 * RTC.c
 *
 * Created: 3/29/2012 3:35:04 PM
 *  Author: Dubs
 */ 
#include<avr\io.h>
#include<avr\sfr_defs.h>                       // The library files are where the definitions for the word DDR, PORT, PIN etc. are stored. 
#include<util\delay.h>                          // This is for using the _delay_ms() function.
#include<avr\interrupt.h>
#include<string.h>

#include "USART\USART.h"

void initPIR()
{
	PORTB |= (1 << PORTB0);
	//Timer
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 0xF423;
	
	//Interrupt
	//PCICR |= (1 << PCIE0);
	//PCMSK0 |= (1 << PCINT0);
}

uint8_t ctoi( char ascii)
{
	return ascii - 48;
}

volatile char command[15];
volatile uint8_t set = 1;



int main(void)
{
	uint8_t hours;
	uint8_t min;
	uint8_t dim;
	uint8_t i = 0;
	char cmd[15];
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT9);
	command[0] = '\0';
	initPIR();
	USART_Init(MYUBRR);
	sei();
	print("Start");
	while(1)
	{
		if(!set)
		{
			strcpy(cmd, command);
			for(i = 0; i < strlen(cmd); ++i)
			{
				switch (cmd[i])
				{
					case 'C':
						hours = ctoi(cmd[i+2])*10 + ctoi(cmd[i+3]);
						min = ctoi(cmd[i+5])*10 + ctoi(cmd[i+6]);
						print("Clock set to %d:%d", hours, min );
						i = i + 6;
						break;
					default:
						break;//do nothing
				}
			}
			set = 1;		
		}		
		_delay_ms(1000);
	}
	return 0;
}



ISR(USART_RX_vect)
{
	/* Wait for data to be received */
	/* Get and return received data from buffer */
	
	uint8_t i = 0;
	unsigned char received = 0;
	while ( !(UCSR0A & (1<<RXC0)) );
	received = UDR0;
	while(received != ';')
	{
		//print("!");
		if( received != 10)
		{
			//print(" %c at %d", received, i);
			command[i] = received;			
			i++;
			while ( !(UCSR0A & (1<<RXC0)) );
			received = UDR0;
		}
	}
	command[i] = '\0';
	set = 0;
	print(command);
} 


ISR(__vector_default){} //prevents microcontroller from resetting