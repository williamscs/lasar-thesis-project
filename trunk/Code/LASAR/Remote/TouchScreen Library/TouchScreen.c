/*
 * TouchScreen.c
 *
 * Created: 2/22/2012 4:23:48 PM
 *  Author: Dubs
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include "TouchScreen.h"

void initTouchScreen(void)
{
	//really this just initializes the ADC
	ADMUX = 0;
	//Tells the ADC to use the AREF voltage
	ADMUX |= (1 << REFS0);
	//Sets Prescaler to 128 (ADPS2..0) and Enables the ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);  
	//Free running mode
	ADCSRB = 0;
}



uint16_t getX()
{
	TS_DIR = 0;							//Cleared to all inputs
	TS_DIR |= (1 << Y1) | (1 << Y2);	//Y1 and Y2 set to outputs
	TS_PORT &= ~(1 << Y1);				//Y1 is set to 0V (GND)
	TS_PORT |= (1 << Y2);				//Y2 is set to 5V (VCC)
	
	analogRead( X2 );
}

uint16_t getY()
{
	TS_DIR = 0;							//Cleared to all inputs
	TS_DIR |= (1 << X1) | (1 << X2);	//X1 and X2 are outputs
	TS_PORT &= ~(1 << X2);				//X2 is set to 0V (GND)
	TS_PORT |= (1 << X1);				//X1 is set to 5V (VCC)
	
	analogRead( Y1 );
}

uint16_t analogRead( uint8_t pin )
{
	ADMUX |= (0x0F & pin);				//pin set with safety mask
	ADCSRA |= (1 << ADSC);				//Start the ADC calculation 
	while( ADCSRA & (1<<ADSC) );		//wait until ADC is complete
	
	return ADC;
}