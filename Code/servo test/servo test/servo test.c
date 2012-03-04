/******************************************************************************

Program to demonstrate the use servo motors with AVR Microcontrollers.

Servo Motor: HITEC HS-645MG (360 Modified)
Servo Control PIN (white): To OC1A PIN
Crystal: 16MHz
EXTENDED Fuse: 0xFF
LOW Fuse: 0xD9
HIGH Fuse: 0xF7

WRITTEN BY:
DERON JAKUB
jakubderon@gmail.com
*******************************************************************************/

#include <avr/io.h>
#include <avr\sfr_defs.h>  
#define F_CPU 16000000UL
#include <util/delay.h>

//Simple Wait Function
void Wait()
{
   uint8_t i;
   for(i=0;i<50;i++)
   {
      _delay_loop_2(0);
      _delay_loop_2(0);
      _delay_loop_2(0);
   }
}

int main(void)
{

	//Port D pins as input
	DDRD=0x00;

	//Enable internal pull ups
	PORTD=0xFF;

	//Set PORTB1 pin as output
	DDRB=0xFF;

	/*=========================================TIMERS===========================================
	Timer/Counter1 Control Register A - TCCR1A
	bit    7    6     5    4     3       2     1    0
	   ________________________________________________________________________
	   | COM1A1 | COM1A0 | COM1B1 | COM1B0 | COM1C1 | COM1C0 | WGM11 | WGM10 |
	   ------------------------------------------------------------------------
	Read/Write    R/W  R/W   R/W   R/W     R/W   R/W   R/W   R/W  
	Intial Value 0   0    0     0   0    0     0    0

	Timer/Counter1 Control Register B - TCCR1B
	bit    7    6     5    4     3       2     1    0
	   __________________________________________________________________________
	   | ICNC1  | ICES1  |   -   |  WGM13  |  WGM12  |  CS12  |  CS11  |  CS10  |
	   ------------------------------------------------------------------------
	Read/Write    R/W  R/W    R     R/W     R/W   R/W   R/W   R/W  
	Intial Value 0   0    0     0   0    0     0    0
	===============================================================================================
	*/
	//TOP=ICR1;
	//Output compare OC1A 8 bit non inverted PWM
	//Clear OC1A on Compare Match, set OC1A at TOP
	//Fast PWM
	//ICR1=20000 defines 50Hz PWM

	ICR1=20000;
	//TCCR1A = 01000010
	TCCR1A = 0;
	TCCR1A|=(1<<COM1A1) | (1<<WGM11);
	//TCCR1B = 00011010
	TCCR1B = 0;
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11);
	//start timer with prescaler 8  //16Mhz/8 = 2MHz

	 for (;;) 
	 {
		 Wait();  
		 OCR1A = 0x3FFF;
			// set PWM for 25% duty cycle @ 16bit

		OCR1B = 0xBFFF;
			// set PWM for 75% duty cycle @ 16bit
 
		 //increase duty cycle
		 //OCR1A+=10;
/*
		 if(bit_is_clear(PIND, 1)) 
		 
			 //decease duty cycle
			 OCR1A-=10;
			 loop_until_bit_is_set(PIND, 1);
		 }
		 */
	}

}