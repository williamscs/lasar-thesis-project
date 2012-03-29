/*
 * RTC.c
 *
 * Created: 3/29/2012 3:35:04 PM
 *  Author: Dubs
 */ 

#define F_CPU 16000000UL

#include<avr\io.h>
#include<avr\sfr_defs.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include "SPI\spi.h"
#include "USART\USART.h"

#define FOSC F_CPU    // Clock Speed
#define BAUD 9600UL
#define MYUBRR (F_CPU/(16*BAUD))-1

void RTC_init()
{
	  // start the SPI library:
	  setup_spi(SPI_MODE_1, SPI_MSB, SPI_NO_INTERRUPT, SPI_MSTR_CLK16);
	  //set control register 
	  PORTD &= ~(1 << PORTD5);  
	  send_spi(0x8E);
	  send_spi(0x60);
	  //0b01100000
	  
	  PORTD |= (1 << PORTD5);
	  _delay_ms(10);
}

void SetTimeDate(int d, int mo, int y, int h, int mi, int s)
{ 
	int TimeDate [7]={s,mi,h,0,d,mo,y};
	for(int i=0; i<=6;i++)
	{
		if(i==3)
			i++;
		
		int b= TimeDate[i]/10;
		int a= TimeDate[i]-b*10;
		
		if(i==2)
		{
			if (b==2)
				b=0b00000010;
			else if (b==1)
				b=0b00000001;
		}	
		TimeDate[i]= a+(b<<4);
		
		PORTD &= ~(1 << PORTD5);
		send_spi(i+0x80);
		send_spi((uint8_t)TimeDate[i]);
		PORTD |= (1 << PORTD5);
	}
}

char* ReadTimeDate()
{
	
	char this[5];
	char * temp = "";
	int TimeDate [7]; //second,minute,hour,null,day,month,year
	//int TimeDate[7] = {1,2,3,4,5,6,7};
	for(int i=0; i<=6;i++)
	{
		if(i==3)
			i++;
		PORTD &= ~(1 << PORTD5);
		send_spi(i+0x00); 
		uint8_t n = send_spi(0x00);
		//print(itoa(n,this, 10));
		PORTD |= (1 << PORTD5);
		int a=n & 0b00001111;    
		if(i==2)
		{	
			int b=(n & 0b00110000)>>4; //24 hour mode
			if(b==0b00000010)
				b=20;        
			else if(b==0b00000001)
				b=10;
			TimeDate[i]=a+b;
		}
		else if(i==4)
		{
			int b=(n & 0b00110000)>>4;
			TimeDate[i]=a+b*10;
		}
		else if(i==5)
		{
			int b=(n & 0b00010000)>>4;
			TimeDate[i]=a+b*10;
		}
		else if(i==6)
		{
			int b=(n & 0b11110000)>>4;
			TimeDate[i]=a+b*10;
		}
		else
		{	
			int b=(n & 0b01110000)>>4;
			TimeDate[i]=a+b*10;	
		}
	}
	strcat(temp, itoa(TimeDate[4], this, 10));
	strcat(temp, "/");
	strcat(temp, itoa(TimeDate[5], this, 10));
	strcat(temp, "/");
	strcat(temp, itoa(TimeDate[6], this, 10));
	strcat(temp, "     ");
	strcat(temp, itoa(TimeDate[4], this, 10));
	strcat(temp, ":");
	strcat(temp, itoa(TimeDate[1], this, 10));
	strcat(temp, ":");
	strcat(temp, itoa(TimeDate[0], this, 10));
	
	return temp;
}

int main(void)
{
	char this[5];
	itoa(23, this, 10);
	RTC_init();
	USART_Init(MYUBRR);
	SetTimeDate(29, 3, 2012,3,49,34);
	
	while(1)
	{
		
		print("New Line!");
		ReadTimeDate();
		_delay_ms(500);
	}
	return 0;
}
