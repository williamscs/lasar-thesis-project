/*
 * Satellite.c
 *
 * Created: 2/1/2012 4:17:34 PM
 *  Author: Dubs
 */ 

#define F_CPU 16000000UL

#include<avr\io.h>
#include<avr\sfr_defs.h>                       // The library files are where the definitions for the word DDR, PORT, PIN etc. are stored. 
#include<util\delay.h>                          // This is for using the _delay_ms() function.
#include<avr\interrupt.h>
#include<avr\sleep.h>


//Function headers
void initTimer( int dutycycle);
void initInterrupt0();
void setCycle(int dutycycle);

//USART Stuff
#define FOSC 16000000UL    // Clock Speed
#define BAUD 9600UL
#define MYUBRR (F_CPU/(16*BAUD))-1 

volatile unsigned int dim = 20;
volatile unsigned int count = 0;
volatile uint8_t rxflag = 0;
volatile uint8_t slpflag = 0;
volatile uint8_t zerocross = 1;

int main(void)
{
	DDRB = (1<<PORTB0);
	DDRC = 0xFF;
	//DDRB = (1 << PORTD6);
	//DDRD = (1 << PORTD3);
	DDRD &= ~(1 << PORTD2);
	PORTB &= ~(1 << PORTB0);
	//PORTD &= (1 << PORTD6);
	
	initTimer(65);
	initInterrupt0();
	dim = 10;
	
	// turn on interrupts
	sei();
	
	while(1)
	{
		for( int j = 1; j < 90; ++j )
		{
			//PORTD |= (1 << PORTD3);
			dim = j;
			PORTC = j;
			_delay_ms(40);
		}
		for( int j = 90; j > 1; --j)
		{
			//PORTD &= ~(1 << PORTD3);
			dim = j;
			PORTC = j;
			_delay_ms(40);
		}
	}				
    return(0);
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
	//Enable OVF
	
	/* OLD CODE 2/15/2012
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

    TCCR0B |= (1 << CS01);
    // set prescaler to 8 and starts PWM
	
	TIMSK0 = (1 << OCIE0B);
	//Enable COMPA
	*/
}


void initInterrupt0()
{
	PORTD |= (1 << PORTD2) | (1 << PORTD3);
	EICRA = 0;
	EICRA |= (1 << ISC11) | (1 << ISC01);
	EIMSK |= (1 << INT1) | (1 << INT0);
}	


void setCycle(int dutycycle)
{
	cli();
	OCR0A = dutycycle;
	sei();
	return;
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
	if( zerocross == 1) 
	{
		if( count >= dim )
		{
			//PORTD |= (1 << PORTD6);
			PORTB |= (1 << PORTB0);
			_delay_us( 5 );
			//PORTD &= ~(1 << PORTD6);
			PORTB &= ~(1 << PORTB0);
			count = 0;
			zerocross = 0;
		}
		else
		{
			
			count = count + 1;
		}
	}
}

ISR(INT0_vect)
{
	zerocross = 1;
}

ISR(INT1_vect)
{
	//Go to low power state
	if(slpflag == 0)
	{
		PORTB &= ~(1 << PORTB0);
		set_sleep_mode( SLEEP_MODE_PWR_SAVE );
		sleep_enable();
		sleep_cpu();
		slpflag = 1;
	}		
	else
	{
		sleep_disable();
		slpflag = 0;
	}		
}