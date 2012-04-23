/*
 * DS3234.c
 *
 * Created: 3/20/2012 5:15:36 PM
 *  Author: Dubs
 */ 
#include <util/delay.h>
#include <string.h>
#include "SPI\spi.h"

void RTC_init()
{
	  // start the SPI library:
	  setup_spi(SPI_MODE_1, SPI_MSB, SPI_NO_INTERRUPT, SPI_MSTR_CLK16);
	  //set control register 
	  send_spi(0x8E);
	  send_spi(0x60);
	  //0b01100000
	  
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
	char * temp = "";
	int TimeDate [7]; //second,minute,hour,null,day,month,year
	//int TimeDate[7] = {1,2,3,4,5,6,7};
	for(int i=0; i<=6;i++)
	{
		if(i==3)
			i++;
		send_spi(i+0x00); 
		uint8_t n = send_spi(0x00);
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
	
	return temp;
}
