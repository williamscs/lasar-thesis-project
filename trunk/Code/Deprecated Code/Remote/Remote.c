/*
 * Remote.c
 *
 * Created: 2/1/2012 4:20:53 PM
 *  Author: Chris Williams
 */ 

#define F_CPU 10000000UL

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <avr\interrupt.h>
#include "Remote.h"
#include "USART\USART.h"
#include "TouchScreen Library\TouchScreen.h"
#include "LCD Library\typedefs.h"
#include "LCD Library\GrLCD.h"
#include "FATFS Library\ff.h"
#include "FATFS Library\mmc.h"

//USART Stuff
#define FOSC F_CPU    // Clock Speed
#define BAUD 9600UL
#define MYUBRR (F_CPU/(16*BAUD))-1




/******************GLOBAL VARIABLES******************/

//FATFS Variables
FRESULT f_err_code;
static FATFS FATFS_Obj;

//LCD Library Variables
//String_T strg = { 0, 0, 0, 0, 0, 0, 0 };	// string structure for text display
char txtbuf[40];

/****************END GLOBAL VARIABLES****************/

int main(void)
{
	DIR_MD = OUTS;		// Mode Port, Output
	CTRL = 0xFF;		// all control bits high
	DIR_CT = OUTS;		// Control Port, Output
	DIR_HI = OUTS;		// Data 8-15, Output
	DIR_LO = OUTS;		// Data 0-7, Output
	
	MODE = M80_16;		// 8080 16 bit mode

	//LCDInit();
	
	display_rgb(BLUE);	// paint background color
	//USART_Init(MYUBRR);
	//init10msTimer();
	//disk_initialize(0);
	DDRC |= (1 << DDC0);
	
	//f_err_code = f_mount(0, &FATFS_Obj);
	//if( f_err_code != FR_OK )
	//{
		////SD Card not read properly (or not inserted)
	//}		
    while(1)
    {
		PORTC ^= (1 << PORTC0);
		_delay_ms(500);
		/* Sleep Code - Need to implement proper start logic
		set_sleep_mode( SLEEP_MODE_PWR_SAVE );
		PORTB &= ~(1 << PORTB0);
		slpflg = 1;
		EIMSK &= ~(1 << INT0);
		sleep_enable();
		sleep_cpu();
		sleep_disable();
		*/
		//
        //TODO:: Please write your application code 
    }
}


void init10msTimer()
{
	//CTC Mode
	TCCR0A = (1 << WGM01);
	//prescaler set to 1024
	TCCR0B = (1 << CS02) | (1 << CS00);
	
	//10ms Timer requires frequency of 100Hz
	// used avrcalc to receive value of OCR
	OCR0A = 0x9B;
}

void display_rgb(WORD data)
{
	WORD i,j;

	LCDSetGCur(0, 0);
	for(i = 0; i < NR_ROWS; i++)
	{
		for(j = 0; j < NR_COLS; j++)
		{
			LCDData(data);
		}
	}
}

void USART_Init( unsigned int ubrr)
{
/*Set baud rate */
    UBRR1H = (ubrr >> 8);
    UBRR1L = ubrr;
    
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);      // Enable receiver and transmitter and interrupt receive
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);    // Set frame: 8data, 1 stp
}


void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
    while ( !(UCSR1A & (1 << UDRE1)) ) // Wait until buffer is empty

	;
	/* Put data into buffer, sends the data */
	UDR1 = data;
}


unsigned char USART_Receive( void )
{
	return '\0';
}

/* Interrupt Service Routines */

ISR(INT0_vect)
{
	
}

ISR(TIMER0_OVF_vect)
{
	//disk_timerproc();
}