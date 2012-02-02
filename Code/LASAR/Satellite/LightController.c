/*
 * LightController.c
 *
 * Created: 11/5/2011 1:46:08 PM
 *  Author: Chris Williams
 */ 

#define F_CPU 16000000UL
#include <avr\io.h>
#include<avr\sfr_defs.h>                       // The library files are where the definitions for the word DDR, PORT, PIN etc. are stored. 
#include<util\delay.h>                          // This is for using the _delay_ms() function.
#include<avr\interrupt.h>
#include <avr\pgmspace.h> 
#include "lcd.h"

//USART Stuff
#define FOSC 16000000UL    // Clock Speed
#define BAUD 9600UL
#define MYUBRR  0x0067  

//#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
//#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

void ledshow();
void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
volatile unsigned int dim = 0;
volatile unsigned int tick = 0;
volatile uint8_t rxflag = 0;

//Irrelevant for 
const uint8_t  sinewave[] PROGMEM= //256 values 
{ 

0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,0x98,0x9c,0x9f,0xa2,0xa5,0xa8,0xab,0xae, 
0xb0,0xb3,0xb6,0xb9,0xbc,0xbf,0xc1,0xc4,0xc7,0xc9,0xcc,0xce,0xd1,0xd3,0xd5,0xd8, 
0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xed,0xef,0xf0,0xf2,0xf3,0xf5, 
0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfc,0xfd,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff, 
0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfd,0xfc,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7, 
0xf6,0xf5,0xf3,0xf2,0xf0,0xef,0xed,0xec,0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc, 
0xda,0xd8,0xd5,0xd3,0xd1,0xce,0xcc,0xc9,0xc7,0xc4,0xc1,0xbf,0xbc,0xb9,0xb6,0xb3, 
0xb0,0xae,0xab,0xa8,0xa5,0xa2,0x9f,0x9c,0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83, 
0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x67,0x63,0x60,0x5d,0x5a,0x57,0x54,0x51, 
0x4f,0x4c,0x49,0x46,0x43,0x40,0x3e,0x3b,0x38,0x36,0x33,0x31,0x2e,0x2c,0x2a,0x27, 
0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,0x15,0x13,0x12,0x10,0x0f,0x0d,0x0c,0x0a, 
0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x03,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00, 
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x05,0x06,0x07,0x08, 
0x09,0x0a,0x0c,0x0d,0x0f,0x10,0x12,0x13,0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23, 
0x25,0x27,0x2a,0x2c,0x2e,0x31,0x33,0x36,0x38,0x3b,0x3e,0x40,0x43,0x46,0x49,0x4c, 
0x4f,0x51,0x54,0x57,0x5a,0x5d,0x60,0x63,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c 

};


int main() 
{
	sei();
    DDRD = 0b00100000;                   // DDR is used to set the I/O  0-> Input 1-> Output ... here PC0 is set as output.
    PORTD = 0;                                // This initializes the port pins
	DDRC = 0xFF;
	PORTC = 0;
	int i = 0;
	uint8_t rxed;
	USART_Init( MYUBRR );
	lcd_init();
	_delay_ms(100);
	lcd_string("    ");
	lcd_clrscr();
	_delay_ms(1000);
	

	//lcd_string("123456789a123456789b123456789c123456789d123456789e123456789f123456789g123456789h");
	//lcd_string2("Look ma!\nI'm on uStream!");
	//lcd_string2("%cC > %cP\nJust sayin\'",228,228);
	lcd_string2("Chris Williams \nCEEN 4360-001");
	_delay_ms(500);
	ledshow();
	//lcd_setline(2);
	//lcd_string("Come on...");
	//led_pattern();

	
	//for (int i=0;i<10;i++) //some delay
		_delay_ms(500);
	//PORTC = 0;
	//lcd_clrscr();
	_delay_ms(500);
	lcd_clrscr();
	PORTD |= (1<<PORTD5) | (1<<PORTD6);
	lcd_clrscr();
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
	
	
	
	
	
	lcd_string2( "Your light is:");
	while(1)
	{
		
		lcd_setline(1);
		_delay_ms(10);
		//USART_Transmit(':');
		//USART_Transmit('(');
		//lcd_string2("%d", i );
		//rxed = USART_Receive();
		//USART_Transmit(rxed);
		if(rxflag == 1)
		{	 
			//Reset Tone Timer
			
			OCR0A = dim; // 10 out of 256 means very short on period (low voltage)
			TCCR1B |= (1<<CS11);
			if( dim >= 200)
			{
			
				lcd_string("High");
				TCNT1 = 0;
				TCCR1A=0x31;
				_delay_ms(50);
				TCCR1A = 0x0;
				_delay_ms(10);
				TCCR1A=0x31;
				_delay_ms(50);
				TCCR1A = 0x0;
				_delay_ms(10);
				TCCR1A=0x31;
				_delay_ms(50);
				TCCR1A = 0x0;
			}				
			else if( dim >= 100 )
			{
				lcd_string("Mid ");
				TCNT1 = 0;
				TCCR1A=0x31;
				_delay_ms(500);
				TCCR1A = 0x0;
				
			}				
			else if( dim > 0 )
			{
				lcd_string("Dim ");
				TCNT1 = 0;
				TCNT1 = 0;
				TCCR1A=0x31;
				_delay_ms(250);
				TCCR1A = 0x0;
				_delay_ms(250);
				TCCR1A=0x31;
				_delay_ms(250);
				TCCR1A = 0x0;
			}				
			else if( dim == 0 )
			{
				lcd_string("Off ");
				
			}				
			rxflag = 0;
			
			_delay_ms(10);
			TCCR1A=0x31;
			_delay_ms(250);
			TCCR1A = 0x0;
		
		}
		//lcd_string2("%d", dim);
		_delay_ms(500);
		tick++;
		if(tick==250)
			tick=0;
			// going to use PORTC.0 to PWM the contrast voltage 
		   
	}	
    return(0);
}

void ledshow()
{
	for(int j=0; j < 3; ++j)
	{
		for(int i = 0; i < 4; ++i)
		{
			PORTC=1<<i;
			_delay_ms(100);
		}
		for(int i = 0; i < 4; ++i)
		{
			PORTC=8>>i;
			_delay_ms(100);
		}
	}
	for (int i = 0; i < 5; ++i)
	{
		PORTC = 0x5;
		_delay_ms(150);
		PORTC = 0xA;
		_delay_ms(150);
	}
	for (int i = 0; i < 0xf; ++i)
	{
		PORTC = i;
		_delay_ms(100);
	}
}


void USART_Init( unsigned int ubrr)
{
/*Set baud rate */
    UBRR0H = (MYUBRR >> 8);
    UBRR0L = MYUBRR;
    
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
   OCR1B=pgm_read_byte(&sinewave[i]); 
   i++; 
} 