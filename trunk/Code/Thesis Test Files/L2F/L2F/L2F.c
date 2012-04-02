/*
 * L2F.c
 *
 * Created: 3/20/2012 5:31:40 PM
 * Author: Deron Jakub
 *
 * Purpose: 
 * - - - Prototype TSL235R
 *
 * Functionality: 
 * - - - The L2F module uses PCINT to track the input frequency 
 *		 of the TSL235R Light-To-Frequency Converter. The Frequency
 *		 is then sent over USART Bluetooth and read by a remote device
 *	
 * Digital Pin Layout Atmega328p  
 * - - - Input  PCINT11 = Pin Change
 *		 Atmega Pin 25, 26
 *
 */ 

#define F_CPU 16000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include "USART.h"

volatile uint8_t rxflag = 0;
//		 int period = 2000;		  //measurement period (mS)
//		 int ScalingFactor = 1;   //Scaling factor of this sensor
//		 float area = 0.0092;     //Sensing area of TSL25R
//		unsigned long counter = 0;//Counter of Measurements
volatile uint8_t active = 0;
volatile uint8_t tInactive = 0;
volatile unsigned long NumChanges = 0; //Counter of Pulses in Each Measurement Period
volatile unsigned int Freq = 0;		  //Store the Frequency
//volatile unsigned int startTime;  //Store the Time When Measurement Starts
//volatile unsigned int currentTime;//Used to measure the time for measurement 
//		 float irradiance;        //Store the Irradiance
char buffer[12];		  

///////////////////////////////		 
void initTimer( int dutycycle )
{
	OCR0A = 130; //cap of Timer0
	OCR0B = dutycycle;
        
	//DDRD |= (1 << PORTD6);         
        
	TCCR0A |= (1 << COM0A1);  // set non-inverting mode
    TCCR0A |= (1 << WGM01);   // set CTC (Clear Timer on Compare) Mode
    TCCR0B |= (1 << CS01);    // set prescaler to 8 and starts PWM
    TIMSK0 = (1 << OCIE0A) | (1 << TOIE0);
		
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 0xF423;
	//OCR1A = 200;
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
///////////////////////////
//---------PCINT Counter---
ISR(PCINT1_vect) 
{
	if(PINC & (1<<PINC3)) //detect Rising Edge
		NumChanges = NumChanges + 1;
		//PORTD ^= (1<<PORTD6);   //LED Alternates ON and OFF
		////delay_ms(2000);
		//PORTC ^= (1 << PORTC2); //[Follows the Input]		
} 
////////////////////////////
//-----Bluetooth Counter----
ISR(USART_RX_vect)
{
	/* Wait for data to be received */
	while ( !(UCSR0A  & (1 << RXC0)) )
	;
	/* Get and return received data from buffer */
	rxflag = 1;
}

///////////////////////////
//-------DELAYS------------
void delay_ms(uint16_t ms)
{
	while( ms ){
		_delay_ms(1);
		ms--;
	}
}
////////////////////////////
//--------MAIN-------------
int main (void)
{ 	
	//-------PORTD
	// PD0	 Output		LED
	// Pins   76543210 
	// Values 00000011
	DDRD |= (1 << PORTD2); 
   
	//-------PORTC 
	// PC3	 Pin26		Input
	// PC2	 Pin25		Output 
	// Pins   543210 
	// Values 001000   
	DDRC |= (1<<DDC0);
	////Enable Pin Change Interrupt on TxIn Pin PC3 
	initTimer(1234);
	
	//PCINT//////////////////////////	
	//Enables PC[23:16] and PC[7:0]
	PCICR = (1 << PCIE2) | (1 << PCIE0);
	PCMSK2 = (1 << PCINT20);
	PCMSK0 = (1 << PCINT0);
	
	//TIMER1/////////////////////////
	TCCR1A |= (1 << COM1A0);				//
	TCCR1B |= (1 << WGM12) | (1 << CS12);	//set prescalar to 256
	TIMSK1 |= (1 << OCIE1A);				//
	OCR1A = 0xF423;							//Timer1 CTC @ 1 sec period
	
	
	////Bluetooth Stuff
	USART_Init(MYUBRR);
	sei(); 
	//_delay_ms(1000);
   
	while(1)
	{
		if(tInactive >= 15)
		{
			PORTC &= ~(1 << PORTC0);
		}			
		itoa(Freq,buffer,10);
		strcat(buffer,"kHz ");
		USART_Print(buffer);
		_delay_ms(100);
		//USART_Transmit('e');
		//USART_Print("Hello");
		//itoa(tInactive, buffer, 10);
		//strcat(buffer, "s ");
		//USART_Print(buffer);
		//_delay_ms(500);
	}
	return 0;

} //end main 



ISR(TIMER1_COMPA_vect)
{
    Freq = NumChanges/1000;
	NumChanges = 0;
	
	if(!active)
		tInactive++;
}

/*PIR Sensor
ISR(PCINT0_vect)
{
	if( !(PINB & (1 << PORTB0)) && !active)
	{
		USART_Print("Tripped Up");
		active = 1;
		
		PORTC |= (1 << PORTC0);
		tInactive = 0;
	}
	else if((PINB & (1<<PORTB0)) && active)
	{
		USART_Print("Tripped Down");
		active = 0;
	
	}
}
*/

ISR(PCINT2_vect)
{
	if(PIND & (1<< PORTD4))
		NumChanges++;
	//_delay_ms(50);
	
}

ISR(__vector_default){}