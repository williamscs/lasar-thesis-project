/*
 * Satellite.c
 *
 * Created: 2/1/2012 4:17:34 PM
 *  Author: Chris Williams
 */ 

#define F_CPU 16000000UL

#include <avr\io.h>
#include <avr\sfr_defs.h>        // The library files are where the definitions for the word DDR, PORT, PIN etc. are stored. 
#include <util\delay.h>          // This is for using the _delay_ms() function.
#include <avr\interrupt.h>
#include <avr\sleep.h>
#include "Servo\Servo.h"
#include "USART\USART.h"


//Function headers
void initTimer( int dutycycle);
void initInterrupts();
void setCycle(int dutycycle);

//USART Stuff
#define FOSC F_CPU    // Clock Speed
#define BAUD 9600UL
#define MYUBRR (F_CPU/(16*BAUD))-1

//Global Variables
volatile unsigned int dim = 20;
volatile unsigned int count = 0;
volatile uint8_t rxflag = 0;
volatile uint8_t slpflg = 0;
volatile uint8_t zerocross = 1;
volatile uint32_t freqCounter = 0;
volatile uint32_t frequency = 0;

int main(void)
{
	DDRC = 0xFF;
    DDRD |= (1 << PORTD6); 
	DDRB = 0;
	PORTD &= ~(1 << PORTD6);
	PORTD |= (1 << PORTD6);
	//_delay_ms(1000);
	
	initTimer(65);
	initInterrupts();
	initServo(SERVO_PERIOD);
	USART_Init(MYUBRR);
	dim = 10;
	
	// turn on interrupts
	sei();
	
	while(1)
	{
		PORTC &= ~(1 << PORTC0);
		/*
		set_servo1(SERVO_FWD);
		PORTC = (1 << PORTC0);
		//_delay_ms(1000);
		set_servo1(SERVO_REV);
		PORTC &= ~(1 << PORTC0);
		//_delay_ms(1000);
		PORTC = (1 << PORTC0);
		set_servo1(SERVO_CEN);
		//_delay_ms(1000);
		*/
		/*for( int j = 10; j < 90; ++j )
		{
			//PORTD |= (1 << PORTD3);
			dim = j;
			PORTC = j;
			
			
			_delay_ms(100);
		}
		for( int j = 90; j > 10; --j)
		{
			//PORTD &= ~(1 << PORTD3);
			dim = j;
			PORTC = j;
			set_servo1(SERVO_REV);
			_delay_ms(100);
			set_servo1(SERVO_CEN);
			_delay_ms(20);
		}*/
		/* Sample Sleep Code - need to remove
		set_sleep_mode( SLEEP_MODE_PWR_SAVE );
		PORTB &= ~(1 << PORTB0);
		slpflg = 1;
		EIMSK &= ~(1 << INT0);
		sleep_enable();
		sleep_cpu();
		sleep_disable();
		*/
	}	
    return(0);
}

/*
 * Function Name: initTimer
 * Author: Chris Williams
 */
void initTimer( int dutycycle )
{
	OCR0A = 130; //cap of Timer0
	OCR0B = dutycycle;
	 
	
	TCCR0A |= (1 << COM0A1) | (1 << WGM01);
	// set non-inverting mode 
	// set CTC (Clear Timer on Compare) Mode

    TCCR0B |= (1 << CS01);    // set prescaler to 8 and starts PWM
	
	TIMSK0 = (1 << OCIE0A) | (1 << TOIE0);	//Enable OVF

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
	//PORTD |= (1 << PORTD2) | (1 << PORTD3);
	EICRA |= (1 << ISC11) | (1 << ISC01);
	EIMSK |= (1 << INT1) | (1 << INT0);
	
	PCICR = (1 << PCIE2) | (1 << PCIE0);
	PCMSK2 = (1 << PCINT20);
	PCMSK0 = (1 << PCINT0);
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

//OLD: 2/15/2012
//ISR(TIMER0_OVF_vect)
ISR(TIMER0_COMPA_vect)
{
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
		else if( count )
		{
			
			count = count + 1;
		}
	}
}

ISR(TIMER1_COMPA_vect)
{	
	frequency = freqCounter * 50;
	freqCounter = 0;
}

ISR(INT0_vect)
{
	zerocross = 1;
}

ISR(USART_RX_vect)
{
	/* Wait for data to be received */
	while ( !(UCSR0A  & (1 << RXC0)) )
	;
	/* Get and return received data from buffer */
	//dim = UDR0;
	rxflag = 1;
}

//PIR Sensor
ISR(PCINT0_vect)
{
	if( !(PINB & (1 << PORTB0)) )
	{
		//SOMETHING WAS SENSED
		PORTC |= (1 << PORTC0);
		_delay_ms(1000);
		
	}
	else
	{
		PORTC &= ~(1 << PORTC0);
		_delay_ms(1000);
	}
	
}

//Frequency Counter
ISR(PCINT2_vect)
{
	if(PIND & (1 << PORTD3))
	{
		freqCounter++;
	}
}

/*
 *SLEEP EXAMPLE - No longer used
 * 
 */
/*
ISR(INT1_vect)
{	
	//Go to low power state
	sleep_disable();
	EIMSK |= (1 << INT1) | (1 << INT0);
}*/