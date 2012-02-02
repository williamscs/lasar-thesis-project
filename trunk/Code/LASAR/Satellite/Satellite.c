/*
 * Satellite.c
 *
 * Created: 2/1/2012 4:17:34 PM
 *  Author: Dubs
 */ 

#include <avr\io.h>
#include<avr\sfr_defs.h>                       // The library files are where the definitions for the word DDR, PORT, PIN etc. are stored. 
#include<util\delay.h>                          // This is for using the _delay_ms() function.
#include<avr\interrupt.h>
#include <avr\pgmspace.h> 


//USART Stuff
#define FOSC 16000000UL    // Clock Speed
#define BAUD 9600UL
#define MYUBRR (F_CPU/(16*BAUD))-1 

volatile unsigned int dim = 0;
volatile unsigned int tick = 0;
volatile uint8_t rxflag = 0;

int main(void)
{
	sei();
    DDRD = 0b00100000;                   // DDR is used to set the I/O  0-> Input 1-> Output ... here PC0 is set as output.
    PORTD = 0;                                // This initializes the port pins
	DDRC = 0xFF;
	PORTC = 0;
	int i = 0;
	uint8_t rxed;
	USART_Init( MYUBRR );
	
	PORTD |= (1<<PORTD5) | (1<<PORTD6);
	_delay_ms(500);
	
	//PWM Stuff
	TCCR0A |= (1<<WGM01) | (1<<WGM00); // Configure timer 1 for CTC mode
	TIMSK0 |= ((1<<OCIE0A) | (1<<TOIE0)); // use both interrupts
	TCCR0B |= (1<<CS00); // timer on - nice high PWM frequency
	TCNT0 = 0;	//Initialize timer
	
	DDRB=(1<<2); 
	// initial OCR1B value 
	//OCR1B=80; 
	//COM1B0 and COM1B1 enabled 
	TCCR1A=0x0; 
	//prescaler
	//TCCR1B |= (1<<CS12);
	//enable output compare interrupt for OCR1A 
	TIMSK1=(1<<OCIE1B);
	OCR1B = 120;
	
		
    return(0);
}


ISR(TIMER0_COMPA_vect) 
{ 
	PORTD &= ~(1<<PORTD5);
} 

ISR(TIMER0_OVF_vect)
{ 
	if( OCR0A == 0)
		PORTD = 0;
	else
		PORTD |= (1<<PORTD5);
}


ISR(USART_RX_vect)
{
	/* Wait for data to be received */
	while ( !(UCSR0A  & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	dim = UDR0;
	rxflag = 1;
	return UDR0;
}

uint8_t i=0; 
ISR(TIMER1_COMPB_vect){ 
   
} 