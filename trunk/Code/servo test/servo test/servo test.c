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
//#include <avr\sleep.h>
//#include <avr\eeprom.h>
//#include "USART\USART.h"
//#include "Satellite.h"

//Servo Constants
#define SERVO_PERIOD	312
#define SERVO_FWD 31  //1ms
//#define SERVO_CEN 23	//1.5ms (not needed)
#define SERVO_REV 16	//2ms

//Global Variables
volatile unsigned int dim = 20;
volatile unsigned int count = 0;
volatile uint8_t rxflag = 0;
volatile uint8_t slpflg = 0;
volatile uint8_t zerocross = 1;
volatile uint32_t freqCounter = 0;
volatile uint32_t frequency = 0;

void initServo(const int period_64us);
void static inline openWindow(int pwm, int time);	//inline ensures function execution time optimization
void static inline closeWindow(int time);

int main(void)
{
	//Local Variables
	//uint8_t hours = 0;
	//uint8_t minutes = 0;
	//uint8_t seconds = 0;
	
	DDRB &= ~(1 << PORTB0);
	DDRC = 0xFF;
	
	initInterrupts();
	
	// turn on interrupts
	sei();
		
	
	while(1)
	{
		//PORTC &= ~(1 << PORTC0);
		
		//openWindow(SERVO_FWD, 3000);
		_delay_ms(1000);
		
		//PORTC = (1 << PORTC0);
		_delay_ms(1000);		
	}	
    return(0);
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
 
 
void static inline openWindow(int pwm, int time) 
{
	TCCR1B |= (1 << CS10) | (1 << CS12);
	OCR1A = SERVO_FWD;
	delay_ms(time);
	TCCR1B &= 0xF8;
}

void static inline closeWindow(int time) 
{
	TCCR1B |= (1 << CS10) | (1 << CS12);
	OCR1A = SERVO_REV;
	delay_ms(time);
	TCCR1B &= 0xF8;
}