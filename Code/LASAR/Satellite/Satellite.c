/*
 * Satellite.c
 *
 * Created: 2/1/2012 4:17:34 PM
 *  Author: Chris Williams
 */ 

#define F_CPU 16000000UL
//Indicates whether code will be compiled
// for Smart Satellite or RTC-less Satellite
// ......none of them are dumb :(
#define SMART_SAT 1

#include <avr\io.h>
#include <avr\sfr_defs.h>   // The library files are where the definitions for the word DDR, PORT, PIN etc. are stored. 
#include <util\delay.h>     // This is for using the _delay_ms() function.
#include <avr\interrupt.h>
#include <avr\sleep.h>
#include <avr\eeprom.h>
#include "USART\USART.h"
#include "Satellite.h"

//Servo Constants
#define SERVO_PERIOD	312
#define SERVO_FWD 31  //1ms
//#define SERVO_CEN 23	//1.5ms (not needed)
#define SERVO_REV 16	//2ms

//#define set_servo1(m) (OCR1A = m)
#ifdef SMART_SAT
//EEPROM Variables
uint8_t EEMEM Hour;
uint8_t EEMEM Minute;
uint8_t EEMEM Second;
#endif

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
	//Local Variables
	uint8_t hours = 0;
	uint8_t minutes = 0;
	uint8_t seconds = 0;
	
	DDRB &= ~(1 << PORTB0);
	DDRC = 0xFF;
    DDRD |= (1 << PORTD6); 
	//DDRB = 0;
	//_delay_ms(1000);
	
	initTimer(65);
	//initInterrupts();
	//initServo(SERVO_PERIOD);
	USART_Init(MYUBRR);
	dim = 100;
	char * str;
	// turn on interrupts
	sei();
		
	
	while(1)
	{
		//PORTC &= ~(1 << PORTC0);
		//openWindow(3000);
		//PORTC = (1 << PORTC0);
		//closeWindow(3000);
		//USART_Transmit('l');
		//USART_Transmit('o');
		//USART_Transmit('l');
		//_delay_ms(1000);
		//PORTC &= ~(1 << PORTC0);
		//_delay_ms(20);
		//OCR1A = 16;
		//PORTC &= ~(1 << PORTC0);
		////_delay_ms(1000);
		//str = "test";
		//USART_Transmit('1');
		//print(str);
		//PORTD = (1 << PORTD6);
		for( int j = 10; j < 90; ++j )
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
			openWindow(SERVO_REV);
			_delay_ms(20);
		}
		
		
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
	//Falling edge triggers interrupt for INT0 or INT1
	EICRA |= (1 << ISC11) | (1 << ISC01);
	//Enable INT0 and INT1
	EIMSK |= (1 << INT1) | (1 << INT0);
	
	//Enables PC[23:16] and PC[7:0]
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

void initServo(const int period_64us) 
{
	DDRB |= (1 << PORTB1);
	//initialize TMR1 (PWM)
	// clear on compare, fast PWM, TOP=ICR1 (WGM13/WGM12 in TCCR1B)
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	// prescaler 1024 (and WGM12 and WGM13)
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS12); 
	ICR1 = period_64us;
} 
 

void openWindow( int time) 
{
	TCCR1B |= (1 << CS10) | (1 << CS12);
	OCR1A = SERVO_FWD;
	delay_ms(time);
	TCCR1B &= 0xF8;
	_delay_ms(10);
}

void closeWindow(int time) 
{
	TCCR1B |= (1 << CS10) | (1 << CS12);
	OCR1A = SERVO_REV;
	delay_ms(time);
	TCCR1B &= 0xF8;
	_delay_ms(10);
}

void static inline delay_ms(uint16_t this)
{ 
	while(this--)
	{ 
	_delay_ms(1);
	} 
}

void SPI_init(void)
{ 

  //DDRB = ((1<<DDB2)|(1<<DDB1)|(1<<DDB0)); //spi pins on port b MOSI SCK,SS outputs 

  SPCR = ((1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<CPOL)|(1<<CPHA));  // SPI enable, Master, f/16 

} 

char SPI_Transmit(char cData)
{
   SPDR = cData; 
   while(!(SPSR & (1<<SPIF))) 
      ; 
   return SPDR; 
}

void getTime(uint8_t * hours, uint8_t * minutes, uint8_t * seconds)
{
	
}

void print(char * input)
{
	int length = strlen(input);
	for(int i = 0; i < length; ++i)
	{
		USART_Transmit(input[i]);
	}
	
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
		else
		{
			print("Got Here");
			count = count + 1;
		}
	}
	
}

/********* THIS IS UNUSED *********
ISR(TIMER1_COMPA_vect)
{	
	frequency = freqCounter * 50;
	freqCounter = 0;
}
 **********************************/

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
		
		//openWindow(SERVO_REV, 3000);
		//_delay_ms(1000);
		PORTC |= (1 << PORTC0);
		_delay_ms(1000);
		
	}
	else
	{
		;//Do Nothing
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
 *
ISR(INT1_vect)
{	
	//Go to low power state
	sleep_disable();
	EIMSK |= (1 << INT1) | (1 << INT0);
}*/