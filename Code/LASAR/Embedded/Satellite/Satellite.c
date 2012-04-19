/*
 * Satellite.c
 *
 * Created: 2/1/2012 4:17:34 PM
 *  Author: Dubs
 */ 

#include<avr\io.h>
#include<avr\sfr_defs.h>                       // The library files are where the definitions for the word DDR, PORT, PIN etc. are stored. 
#include<util\delay.h>                          // This is for using the _delay_ms() function.
#include<avr\interrupt.h>
#include<stdlib.h>
#include<string.h>
#include<avr/eeprom.h>

#include "Satellite.h"
#include "USART\USART.h"

//USART
volatile uint8_t rxflag = 0;
volatile uint8_t received = 0;
volatile char command[15];
volatile uint8_t set = 1;

//AC
volatile int dim;
volatile int tdim;
volatile unsigned int count = 0;
volatile uint8_t zerocross = 0;

//Blinds
volatile int8_t blinds;
volatile uint8_t sActive = 0;

//PIR Sensor
volatile uint8_t active = 1;
volatile uint8_t tInactive = 0;

//L2F
volatile unsigned long NumChanges = 0; //Counter of Pulses in Each Measurement Period
volatile float Freq = 0;		  //Store the Frequency
volatile int d1;
volatile int d2;

//Realtime Clock
uint8_t chour = 8;
uint8_t cmin = 00;
uint8_t csec = 00;

Pref EEMEM wake;
Pref EEMEM leave;
Pref EEMEM sleep;
Pref EEMEM ret;
Time EEMEM lTime;
uint8_t EEMEM sBlinds = 0;
uint8_t EEMEM sDim = 0;
char buffer[12];

int main(void)
{
	uint8_t hours = 0;
	uint8_t min = 0;
	uint8_t sec;
	uint8_t i = 0;
		
	char cmd[15];
    DDRC |= (1 << DDC0);
	
	initServo();
	initAC(65);
	initL2F();
	initPIR();
	//Bluetooth Init
	USART_Init(MYUBRR);
	// turn on interrupts
    sei();
	int tmp = eeprom_read_byte(&sBlinds);
	if( tmp > 0)
		setBlinds(eeprom_read_byte(&sBlinds));
	else
		setBlinds(0);
	tmp = eeprom_read_byte(&sDim);
	if(tmp > 0)
		setDim(tmp);
	else
		setDim(0);
	#if DEBUG
	print("Start");
	print("Wake set to %d:%d with dim value %d",
		eeprom_read_byte (&wake.hour),
		eeprom_read_byte (&wake.min),
		eeprom_read_byte (&wake.dim),
		eeprom_read_byte (&wake.blinds));
	print("Leave set to %d:%d with dim value %d",
		eeprom_read_byte (&leave.hour),
		eeprom_read_byte (&leave.min),
		eeprom_read_byte (&leave.dim),
		eeprom_read_byte (&leave.blinds));
	print("Return set to %d:%d with dim value %d",
		eeprom_read_byte (&ret.hour),
		eeprom_read_byte (&ret.min),
		eeprom_read_byte (&ret.dim),
		eeprom_read_byte (&ret.blinds));
    print("Sleep set to %d:%d with dim value %d",
		eeprom_read_byte (&sleep.hour),
		eeprom_read_byte (&sleep.min),
		eeprom_read_byte (&sleep.dim),
		eeprom_read_byte (&sleep.blinds));
	#endif
    while(1)
    {
		checkPIR();
		if(!set)
		{
			tInactive = 0;
			set = 1;
			strcpy(cmd, (const char*)command);
			for(i = 0; i < strlen(cmd); ++i)
			{
				switch (cmd[i])
				{
					case 'C':	//Clock
						hours = getDigits(cmd, &i);
						min = getDigits(cmd, &i);
						sec = getDigits(cmd, &i);
						#if DEBUG
						print("Clock set to %d:%d:%d", hours, min, sec );
						#endif
						i++;
						if(cmd[i+1] == '\0')	//Setting current time
						{
							#if DEBUG
							print("Time on chip: %02d:%02d:%02d\n", chour, cmin, csec);
							#endif
							chour = hours;
							cmin = min;
							csec = sec;
							#if DEBUG
							print("New Time: %02d:%02d:%02d", chour, cmin, csec);
							#endif
							
						} 				
						break;
					case 'D':	//Dimness
						setDim(getDigits(cmd, &i));
						tdim = dim;
						i++;
						break;
					case 'B':	//Blinds
						setBlinds(getDigits(cmd, &i));
						int8_t cmpBlinds = (int8_t)eeprom_read_byte(&sBlinds) - blinds;
						
						if(cmpBlinds != 0)
						{
							varyBlinds(cmpBlinds);
						}
						eeprom_write_byte(&sBlinds, (uint8_t)blinds);
						i++;
						break;
					case 'O':	//Toggle On/Off
						if(cmd[1] == 'F')	//OFF
						{
							setDim(-1);
							i = i + 2;
						}							
						else if (cmd[1] == 'N')	//ON
						{
							setDim(MAX);
							i++;
						}
						print("Dim set to %d", dim);
						break;
					case 'W':
						#if DEBUG
						print("Wake!");
						#endif
						eeprom_write_byte (&wake.hour, hours); 
						eeprom_write_byte (&wake.min, min); 
						eeprom_write_byte (&wake.dim, dim);
						eeprom_write_byte (&wake.blinds, blinds);
						i++;
						break;
					case 'L':
						#if DEBUG
						print("Leave!");
						#endif
						eeprom_write_byte (&leave.hour, hours); 
						eeprom_write_byte (&leave.min, min); 
						eeprom_write_byte (&leave.dim, dim);
						eeprom_write_byte (&leave.blinds, blinds);
						i++;
						break;	
					case 'R':
						#if DEBUG
						print("Return!");
						#endif
						eeprom_write_byte (&ret.hour, hours); 
						eeprom_write_byte (&ret.min, min); 
						eeprom_write_byte (&ret.dim, dim);
						eeprom_write_byte (&ret.blinds, blinds);
						i++;
						break;
					case 'S':
						#if DEBUG
						print("Sleep!");
						#endif
						eeprom_write_byte (&sleep.hour, hours); 
						eeprom_write_byte (&sleep.min, min); 
						eeprom_write_byte (&sleep.dim, dim);
						eeprom_write_byte (&sleep.blinds, blinds);
						i++;
						break;
					case 'X':
						print("XD:%d:%d", dim, blinds);
						break;
					case 'F':
						print("F:%d.%04d", d1, d2);
						break;
					case '?':
						print("Wake set to %d:%d with dim value %d",
							eeprom_read_byte (&wake.hour),
							eeprom_read_byte (&wake.min),
							eeprom_read_byte (&wake.dim),
							eeprom_read_byte (&wake.blinds));
						print("Leave set to %d:%d with dim value %d",
							eeprom_read_byte (&leave.hour),
							eeprom_read_byte (&leave.min),
							eeprom_read_byte (&leave.dim),
							eeprom_read_byte (&leave.blinds));
						print("Return set to %d:%d with dim value %d",
							eeprom_read_byte (&ret.hour),
							eeprom_read_byte (&ret.min),
							eeprom_read_byte (&ret.dim),
							eeprom_read_byte (&ret.blinds));
						print("Sleep set to %d:%d with dim value %d",
							eeprom_read_byte (&sleep.hour),
							eeprom_read_byte (&sleep.min),
							eeprom_read_byte (&sleep.dim),
							eeprom_read_byte (&sleep.blinds));
						break;
					default:
						//print("Default case");
						break;//do nothing
				}//switch(cmd[i])
				
			}//for(each command)		
		}//if(!set)
		
		
		//Clock Logic
		if( csec >= 60)				//Seconds no greater than 60
		{
			csec = 0;
			cmin++;
			if( cmin >= 60)			//Minutes no greater than 60
			{
				cmin = 0;
				chour++;
				if( chour >= 23 )	//Hours no greater than 23
				{
					chour = 0;
				}					
			}
			print("%d:%d:%d", chour,cmin,csec);
			checkAlarm();
		}
		
		if(tInactive > 15 && tInactive < 18)
		{
			if(dim > 0)
				tdim = dim;
			setDim(-1);
		}
		//_delay_ms(1000);
    }//while
	                              
    return(0);
}

void checkAlarm()
{
	uint8_t aHour = eeprom_read_byte(&wake.hour);
	#if DEBUG
	print("Wake hour is: %d", eeprom_read_byte(&wake.hour));
	#endif	
	if(aHour == chour)
	{
		#if DEBUG
		print("Wake minute is: %d", eeprom_read_byte(&wake.min));
		#endif	
		if((eeprom_read_byte(&wake.min))==cmin)
		{
			setDim(eeprom_read_byte(&wake.dim));
			setBlinds(eeprom_read_byte(&wake.blinds));
		}
	}
		
	aHour = eeprom_read_byte(&leave.hour);
		
	if(aHour == chour)
	{
		if((eeprom_read_byte(&leave.min))==cmin)
		{
			setDim(eeprom_read_byte(&leave.dim));
			setBlinds(eeprom_read_byte(&leave.blinds));
		}
	}
		
	aHour = eeprom_read_byte(&ret.hour);
		
	if(aHour == chour)
	{
		if((eeprom_read_byte(&ret.min))==cmin)
		{
			setDim(eeprom_read_byte(&ret.dim));
			setBlinds(eeprom_read_byte(&ret.blinds));
				
		}
	}
		
	aHour = eeprom_read_byte(&sleep.hour);
		
	if(aHour == chour)
	{
		if((eeprom_read_byte(&sleep.min))==cmin)
		{
			setDim(eeprom_read_byte(&sleep.dim));
			setBlinds(eeprom_read_byte(&sleep.blinds));
		}
	}
}

void setDim( int arg )
{
	dim = arg;
	#if DEBUG
	print("Dim set to %d", dim);
	#endif
}

void setBlinds( int arg )
{
	blinds  = arg;
	
	#if DEBUG
	print("Blinds set to %d", blinds);
	#endif
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

uint8_t getDigits(char * cmd, uint8_t * i)
{
	*i = *i +2;
	uint8_t tmp = (cmd[*i]-48);
	if(cmd[*i+1] != '\0' && cmd[*i+1] != ':')
	{
		*i = *i + 1;
		tmp *= 10;				//this is a tens value
		tmp += (cmd[*i]-48);	//add ones
	}		
	return tmp;	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void checkPIR()
{
	if( !active && !(PINB & (1 << PINB0)))
	{
		_delay_ms(25);
		if( !(PINB & (1 << PINB0)) && !active)
		{
			#if DEBUG
			//print(" !!");
			#endif
			active = 1;
		
			tInactive = 0;
			if(dim != tdim)
				setDim(tdim);
		}			
	}
	else if(active && (PINB & (1<<PINB0)))
	{
		//print(" No Motion");
		active = 0;
		
	}	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void initAC(int dutycycle)
{
	DDRC |= (1<<DDC0);
	
    OCR0A = 130; //cap of Timer0
    OCR0B = dutycycle;
                
    //Timer
    //TCCR0A |= (1 << COM0A1);    // set non-inverting mode
    TCCR0A |= (1 << WGM01);     // set CTC (Clear Timer on Compare) Mode
    TCCR0B |= (1 << CS01);      // set prescaler to 8 and starts PWM
    TIMSK0 = (1 << OCIE0A) | (1 << TOIE0);    //Enable OVF
	
	//Interrupt
    EICRA = (1 << ISC01);
    EIMSK |= (1 << INT0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void initL2F()
{
	//Uses same Timer as PIR
	
	//Interrupt
	PCICR |=  (1 << PCIE2);
	PCMSK2 |= (1 << PCINT20);	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void initPIR()
{
	PORTB |= (1 << PORTB0);
	//Timer
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 0xF423;							//sets a 1Hz Timer
	
	//Interrupt
	//PCICR |= (1 << PCIE0);
	//PCMSK0 |= (1 << PCINT0);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
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

/************************************************************************/
/*                                                                      */
/************************************************************************/
void static varyBlinds(int8_t percent) 
{
	//uint8_t tmp = blinds;
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
	#if DEBUG
	print("Blinds closing for %d ms", time);
	#endif	
	delay_ms(time);
	sActive = 1;
	TCCR2B = 0;
	PORTC &= ~(1 << PORTC2);
}

/*
 * INTERRUPT SERVICE ROUTINES
 */

/*PIR Sensor - This is polled now.
//ISR(PCINT0_vect)
//{
	//if( !(PINB & (1 << PORTB0)) && !active)
	//{
		//print("Tripped Up");
		//active = 1;
		//
		//tInactive = 0;
	//}
	//else if((PINB & (1<<PORTB0)) && active)
	//{
		//print("Tripped Down");
		//active = 0;
		//
	//}
	//
//}


/////////////////////////////
////---------PCINT Counter---
//ISR(PCINT1_vect) 
//{
	//if(PINC & (1<<PINC3)) //detect Rising Edge
		//++NumChanges;
		////PORTD ^= (1<<PORTD6);   //LED Alternates ON and OFF
		//////delay_ms(2000);
		////PORTC ^= (1 << PORTC2); //[Follows the Input]		
//} */

/************************************************************************/
/* L2F Detection                                                        */
/************************************************************************/
ISR(PCINT2_vect)
{
	if(PIND & (1<< PORTD4))
		NumChanges++;
	//_delay_ms(50);
	
}

/************************************************************************/
/* Bluetooth Receive                                                    */
/************************************************************************/
ISR(USART_RX_vect)
{
	/* Wait for data to be received */
	/* Get and return received data from buffer */
	
	uint8_t i = 0;
	unsigned char received = 0;
	while ( !(UCSR0A & (1<<RXC0)) );
	received = UDR0;
	while(received != ';')
	{
		#if DEBUG
		//print("!");
		#endif
		if( received != 10)
		{
			#if DEBUG
			//print(" %c at %d", received, i);
			#endif
			command[i] = received;			
			i++;
			while ( !(UCSR0A & (1<<RXC0)) );
			received = UDR0;
		}
	}
	command[i] = '\0';
	set = 0;
	//print((char *)command);
} 
 
//CREATED: 2/15/2012
//AC Lighting - Phase Control
ISR(TIMER0_COMPA_vect)
{
    if( zerocross == 1) 
    {
        if( count >= dim )
        {
			
			PORTC |= (1 << PORTC0);
            _delay_us( 10 );
			PORTC &= ~(1 << PORTC0);
            count = 0;
            zerocross = 0;
        }
        else
        {
                  
            count = count + 1;
        }
    }
}

//L2F - Frequency Calculation
ISR(TIMER1_COMPA_vect)
{
    d1 = NumChanges/1000;
	d2 = NumChanges - d1;
	NumChanges = 0;
	++csec;
	
	if(!active)
	{
		tInactive++;
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

//AC - Zero Cross Detection
ISR(INT0_vect)
{
    zerocross = 1;
}

ISR(__vector_default){} //prevents microcontroller from resetting
	
void static inline delay_ms(uint16_t tick)
{
	for(int i = 0; i < tick; ++i)
		_delay_ms(1);
}