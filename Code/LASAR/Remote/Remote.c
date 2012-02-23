/*
 * Remote.c
 *
 * Created: 2/1/2012 4:20:53 PM
 *  Author: Dubs
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


int main(void)
{
	getX();
    while(1)
    {
        //TODO:: Please write your application code 
    }
}