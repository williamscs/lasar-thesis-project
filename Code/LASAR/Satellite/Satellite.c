/*
 * Satellite.c
 *
 * Created: 2/1/2012 4:17:34 PM
 *  Author: Dubs
 */ 

#include <avr\io.h>
#include <avr\sfr_defs.h>                       // The library files are where the definitions for the word DDR, PORT, PIN etc. are stored. 
#include <util\delay.h>                          // This is for using the _delay_ms() function.
#include <avr\interrupt.h>
#include <string.h>
#include "Satellite.h"
//#include "SPI\spi.h"
#include "RTC\DS3234.h"
#include "USART\USART.h"

volatile uint8_t active = 1;
volatile uint8_t tInactive = 0;
volatile unsigned int dim = 20;
volatile unsigned int count = 0;
volatile uint8_t rxflag = 0;
volatile uint8_t zerocross = 1;
volatile unsigned long NumChanges = 0; //Counter of Pulses in Each Measurement Period
volatile unsigned int Freq = 0;		   //Store the Frequency
char buffer[12];


int main(void)
{
    DDRD = (1 << PORTD5);
	DDRC |= (1 << PORTC0);
	PORTD |= (1 << PORTD5);
    PORTB &= ~(1 << PORTB0);
    //PORTD &= (1 << PORTD6);
        
    //initTimer(65);
    //initInterrupts();
    dim = 10;
	
	//USART_Init(MYUBRR);
    //RTC_init();
	//SPI_init();
	//SetTimeDate(3,20,2012,18,00,50);
    // turn on interrupts
	PORTD |= (1 << PORTD6);
	//PORTC |= (1 << PORTC0);
	//_delay_ms(1000);
        
    while(1)
    {
        for( int j = 1; j < 115; ++j )
        {
            dim = j;
            //_delay_ms(40);
        }
        for( int j = 115; j > 1; --j)
        {
            //PORTC &= ~(1 << PORTC0);
            dim = j;
            //_delay_ms(40);
        }
		
		//print("Hello");
		////_delay_ms(1000);
		////SPI_Transmit('5');
		////_delay_ms(3000);
		//print("Done");
    }                               
    return 0;
}

/*
 * Function Name:
 * Author: Chris Williams
 */
void initTimer( int dutycycle )
{
	OCR0A = 130; //cap of Timer0
	OCR0B = dutycycle;  
        
	 DDRD |= (1 << PORTD6);         
        
        TCCR0A |= (1 << COM0A1);
    // set non-inverting mode

    TCCR0A |= (1 << WGM01);
    // set CTC (Clear Timer on Compare) Mode

    TCCR0B |= (1 << CS01);
    // set prescaler to 8 and starts PWM
        
        TIMSK0 = (1 << OCIE0A) | (1 << TOIE0);
	//TCCR1A |= (1 << COM1A0);				//
	//TCCR1B |= (1 << WGM12) | (1 << CS12);	//set prescalar to 256
	//TIMSK1 |= (1 << OCIE1A);				//
	//OCR1A = 0xF423;							//Timer1 CTC @ 1 sec period
        //
        /* OLD CODE 2/15/2012
        TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

    TCCR0B |= (1 << CS01);
    // set prescaler to 8 and starts PWM
        
        TIMSK0 = (1 << OCIE0B);
        //Enable COMPA
        */
}


void initInterrupts()
{
        PORTD |= (1 << PORTD2);
        EICRA = 0b00000010;
        EIMSK |= (1 << INT0);
		
		//Enables PC[23:16] and PC[7:0]
		PCICR = (1 << PCIE2) | (1 << PCIE0);
		PCMSK2 = (1 << PCINT20);
		PCMSK0 = (1 << PCINT0);
}

void getTime(uint8_t * hours, uint8_t * minutes, uint8_t * seconds)
{
	
}


void static inline delay_ms(uint16_t tick)
{
	for(int i = 0; i < tick; ++i)
	{
		_delay_ms(1);
	}
}

/*
 * INTERRUPT SERVICE ROUTINES
 */

ISR(USART_RX_vect)
{
        /* Wait for data to be received */
        while ( !(UCSR0A  & (1 << RXC0)) )
        ;
        /* Get and return received data from buffer */
        //dim = UDR0;
        rxflag = 1;
}
 
 
//OLD: 2/15/2012
//ISR(TIMER0_OVF_vect)
ISR(TIMER0_COMPA_vect)
{
	
	PORTC |= (1 << PORTC0);
    if( zerocross == 1) 
    {
        if( count >= dim )
        {
            PORTD |= (1 << PORTD6);
            //PORTB |= (1 << PORTB0);
            _delay_us( 5 );
            PORTD &= ~(1 << PORTD6);
            //PORTB &= ~(1 << PORTB0);
            count = 0;
            zerocross = 0;
        }
        else
        {      
            count = count + 1;
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    Freq = NumChanges/1000;	
	NumChanges = 0;
	if(!active)
		tInactive++;
}


//PIR Sensor
ISR(PCINT0_vect)
{
	if( !(PINB & (1 << PORTB0)) )
	{
		active = 1;
	}
	else
	{
		active = 0;
		tInactive = 0;
	}
	
}

ISR(INT0_vect)
{
        zerocross = 1;
}

ISR(__vector_default){} //prevents microcontroller from resetting