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


//Function headers
void initTimer( int dutycycle);
void initInterrupt0();
void setCycle(int dutycycle);

//USART Stuff
#define FOSC 16000000UL    // Clock Speed
#define BAUD 9600UL
#define MYUBRR (F_CPU/(16*BAUD))-1 

volatile unsigned int dim = 0;
volatile unsigned int tick = 0;
volatile uint8_t rxflag = 0;

int main(void)
{
	
	// turn on interrupts
	sei();

	initTimer(128);
	while(1)
	{
		for( int i = 0; i < 256; ++i )
		{
			setCycle(i);
			_delay_ms(40);
		}
		
		for( int i = 255; i > 0; --i)
		{
			setCycle(i);
			_delay_ms(40);
		}
	}				
    return(0);
}

void initTimer( int dutycycle)
{
	OCR0A = dutycycle;
    DDRD |= (1 << PORTD6);         
	
	TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

    TCCR0B |= (1 << CS01);
    // set prescaler to 8 and starts PWM
}

void initInterrupt0()
{
	DDRD &= ~(1 << PORTD2);     // Clear the PB0 pin
	PORTD |= (1 << PIND2);
	PCMSK0 |= (1<<PIND2);
	MCUCR = (1<<ISC01) | (1<<ISC00); //falling edge triggers interrupt
}	


void setCycle(int dutycycle)
{
	cli();
	OCR0A = dutycycle;
	sei();
}

/*
 * INTERRUPT SERVICE ROUTINES
 */

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

ISR(INT0_vect)
{
	
}