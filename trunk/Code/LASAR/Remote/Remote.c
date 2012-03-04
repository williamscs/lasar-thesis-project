/*
 * Remote.c
 *
 * Created: 2/1/2012 4:20:53 PM
 *  Author: Chris Williams
 */ 

#define F_CPU 16000000UL

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include "TouchScreen Library\TouchScreen.h"
#include "LCD Library\typedefs.h"
#include "LCD Library\GrLCD.h"
#include "LCD Library\cfaf320.h"
#include "FATFS Library\ff.h"
#include "FATFS Library\diskio.h"


//Function Prototypes
void init10msTimer();
void display_rgb(WORD data);

/******************GLOBAL VARIABLES******************/

//FATFS Variables
FRESULT f_err_code;
static FATFS FATFS_Obj;

//LCD Library Variables
String_T strg = { 0, 0, 0, 0, 0, 0, 0 };	// string structure for text display
char txtbuf[40];

/****************END GLOBAL VARIABLES****************/

int main(void)
{
	init10msTimer();
	disk_initialize(0);
	f_err_code = f_mount(0, &FATFS_Obj);
	if( f_err_code != FR_OK )
	{
		//SD Card not read properly (or not inserted)
	}		
    while(1)
    {
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

ISR(TIMER0_OVF_vect)
{
	disk_timerproc();
}