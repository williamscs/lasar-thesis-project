/*
 * servo test.c
 *
 * Created: 2/1/2012 4:17:34 PM
 *  Author: Chris Williams
 */ 

#define F_CPU 16000000UL

#include <avr\io.h>
#include <avr\sfr_defs.h>   // The library files are where the definitions for the word DDR, PORT, PIN etc. are stored. 
#include <util\delay.h>     // This is for using the _delay_ms() function.
#include <avr\interrupt.h>
//#include <avr\sleep.h>
//#include <avr\eeprom.h>
#include "USART\USART.h"
//#include "Satellite.h"

//Servo Constants
#define SERVO_PERIOD	312
#define SERVO_FWD 0x07  //1ms
//#define SERVO_CEN 23	//1.5ms (not needed)
#define SERVO_REV 0x2F	//2ms

#define CLOSE_TIME 2500

//Global Variables
volatile unsigned int dim = 20;
volatile unsigned int count = 0;
volatile uint8_t rxflag = 0;
volatile uint8_t slpflg = 0;
volatile uint8_t zerocross = 1;
volatile uint32_t freqCounter = 0;
volatile uint32_t frequency = 0;
volatile uint8_t sActive = 0;

void initServo();
void static inline openWindow(int time);	//inline ensures function execution time optimization
void static inline closeWindow(int time);
void static inline delay_ms();

void static inline varyBlinds(int8_t percent) 
{
	int16_t time = percent*(CLOSE_TIME/100);
	sActive = 1;
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
	if(time > 0)
	{
		OCR2A = SERVO_FWD;
	}		
	else
	{
		OCR2A = SERVO_REV;
		time *= -1;
	}
	print("Blinds closing for %d ms", time);
	delay_ms(time);
	sActive = 1;
	TCCR2B = 0;
	PORTC &= ~(1 << PORTC2);
}

int main(void)
{
	//Local Variables
	//uint8_t hours = 0;
	//uint8_t minutes = 0;
	//uint8_t seconds = 0;
	
	DDRC = 0xFF;
	int turn =50;
	//initInterrupts();
	initServo();
	USART_Init(MYUBRR);
	// turn on interrupts
	sei();
	print("\nStart");
	while(1)
	{
		//PORTC &= ~(1 << PORTC0);
		
		_delay_ms(1000);
		print("Close Window");
		varyBlinds(turn);
		_delay_ms(1000);
		turn *= -1;
		
		//PORTC = (1 << PORTC0);
		//_delay_ms(1000);		
	}	
    return(0);
}



void initServo() 
{
	DDRC |= (1 << PORTC2);
	////initialize TMR1 (PWM) 
	//// clear on compare, fast PWM, TOP=ICR1 (WGM13/WGM12 in TCCR1B)
	//TCCR1A = (1 << COM1A1) | (1 << WGM11);
	//// prescaler 1024 (and WGM12 and WGM13)
	//TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS12); 
	//ICR1 = 250;
	TCNT2 = 0;
	
	
	
	TCCR2A = 0;
	//prescalar /1024	
	OCR2A = 1;
	TIMSK2 = (1 << OCIE2A) | (1 << TOIE2);  //Enable OVF
} 
 
 
void static inline openWindow(int time) 
{
	sActive = 1;
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
	OCR2A = SERVO_FWD;
	delay_ms(time);
	sActive = 0;
	TCCR2B = 0;
	PORTC &= ~(1 << PORTC2);
}

void static inline closeWindow(int time) 
{
	sActive = 1;
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
	OCR2A = SERVO_REV;
	delay_ms(time);
	sActive = 0;
	TCCR2B = 0;
	PORTC &= ~(1 << PORTC2);
}

void static inline delay_ms(int dTime)
{
	for( int i = 0; i < dTime; i++)
	{
		_delay_ms(1);
	}
}

ISR(TIMER2_COMPA_vect)
{
	if(sActive)
		PORTC &= ~(1 << PORTC2);
}

ISR(TIMER2_OVF_vect)
{
	if(sActive)
		PORTC |= (1 << PORTC2);
}

ISR(__vector_default){} //prevents microcontroller from resetting