/*
 * Satellite.c
 *
 * Created: 2/1/2012 4:17:34 PM
 *  Author: Chris Williams
 */ 

#include<avr\io.h>
#include<avr\sfr_defs.h>                       /*  The library files are where the definitions for the word DDR, PORT, PIN etc. are stored.  */
#include<util\delay.h>                          /*  This is for using the _delay_ms() function. */
#include<avr\interrupt.h>
#include<stdlib.h>
#include<string.h>
#include<avr/eeprom.h>

#include "Satellite.h"
#include "USART\USART.h"

/* USART */
volatile uint8_t rxflag = 0;	/**< Flag to indicate command received */
volatile uint8_t received = 0;	/**< Byte received from buffer */
volatile char command[15];		/**< Full Command string from Android */
volatile uint8_t set = 1;		/**< Flag to indicate if command has been set */

/* AC */
volatile int dim;				/**< AC Lighting brightness */
volatile int tdim;				/**< "Temp Dim" or the previous brightness level */
volatile unsigned int count = 0;/**< Compares this value vs dim to achieve proper delay */
volatile uint8_t zerocross = 0;	/**< Flag to indicate a zero cross on AC line */

/* Blinds */
volatile int8_t blinds;			/**< Blinds open level */
volatile uint8_t sActive = 0;	/**< Flag to prevent Servo from moving while system is inactive */

/* PIR Sensor */
volatile uint8_t active = 1;	/**< Flag to indicate if system is active */
volatile uint8_t tInactive = 0;	/**< The amount of time the system has been inactive in seconds */

/* L2F */
volatile unsigned long NumChanges = 0;	/**< Counter of Pulses in Each Measurement Period */
volatile float Freq = 0;				/**< Store the Frequency */
volatile int d1;						/**< Thousands of NumChanges */
volatile int d2;						/**< Hundreds of NumChanges */

/* Realtime Clock */
uint8_t chour = 8;				/**< Current Hour */
uint8_t cmin = 00;				/**< Current Minutes */
uint8_t csec = 00;				/**< Current Seconds */

Pref EEMEM wake;				/**< Wake preferences */
Pref EEMEM leave;				/**< Leave preferences */
Pref EEMEM sleep;				/**< Sleep preferences */
Pref EEMEM ret;					/**< Return preferences */
uint8_t EEMEM sBlinds = 0;		/**< Saved Blinds open level */
uint8_t EEMEM sDim = 0;			/**< Saved brightness level */

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
	/* Bluetooth Init */
	USART_Init(MYUBRR);
	/* Turn on interrupts */
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
					case 'C':	/* Clock */
						hours = getDigits(cmd, &i);
						min = getDigits(cmd, &i);
						sec = getDigits(cmd, &i);
						#if DEBUG
						print("Clock set to %d:%d:%d", hours, min, sec );
						#endif
						i++;
						if(cmd[i+1] == '\0')	/* Setting current time */
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
					case 'D':	/* Dimness */
						setDim(getDigits(cmd, &i));
						tdim = dim;
						i++;
						break;
					case 'B':	/* Blinds */
						setBlinds(getDigits(cmd, &i));
						int8_t cmpBlinds = (int8_t)eeprom_read_byte(&sBlinds) - blinds;
						
						if(cmpBlinds != 0)
						{
							varyBlinds(cmpBlinds);
						}
						eeprom_write_byte(&sBlinds, (uint8_t)blinds);
						i++;
						break;
					case 'O':	/* Toggle On/Off */
						if(cmd[1] == 'F')	/* OFF */
						{
							setDim(-1);
							i = i + 2;
						}							
						else if (cmd[1] == 'N')	/* ON */
						{
							setDim(MAX);
							i++;
						}
						print("Dim set to %d", dim);
						break;
					case 'W':/* Wake */
						#if DEBUG
						print("Wake!");
						#endif
						eeprom_write_byte (&wake.hour, hours); 
						eeprom_write_byte (&wake.min, min); 
						eeprom_write_byte (&wake.dim, dim);
						eeprom_write_byte (&wake.blinds, blinds);
						i++;
						break;
					case 'L': /* Leave */
						#if DEBUG
						print("Leave!");
						#endif
						eeprom_write_byte (&leave.hour, hours); 
						eeprom_write_byte (&leave.min, min); 
						eeprom_write_byte (&leave.dim, dim);
						eeprom_write_byte (&leave.blinds, blinds);
						i++;
						break;	
					case 'R': /* Return */
						#if DEBUG
						print("Return!");
						#endif
						eeprom_write_byte (&ret.hour, hours); 
						eeprom_write_byte (&ret.min, min); 
						eeprom_write_byte (&ret.dim, dim);
						eeprom_write_byte (&ret.blinds, blinds);
						i++;
						break;
					case 'S': /* Sleep */
						#if DEBUG
						print("Sleep!");
						#endif
						eeprom_write_byte (&sleep.hour, hours); 
						eeprom_write_byte (&sleep.min, min); 
						eeprom_write_byte (&sleep.dim, dim);
						eeprom_write_byte (&sleep.blinds, blinds);
						i++;
						break;
					case 'X': /* Sync with Android */
						print("XD:%d:%d", dim, blinds);
						break;
					case 'F': /* Get Frequency for Android */
						print("F:%d.%04d", d1, d2);
						break;
					case '?': /* What are the current alarm values? */
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
						#if DEBUG
						print("Default case");
						#endif
						break;/* do nothing */
				}/* switch(cmd[i]) */
				
			}/* for(each command) */		
		}/* if(!set) */
		
		
		/* Clock Logic */
		if( csec >= 60)				/* Seconds no greater than 60 */
		{
			csec = 0;
			cmin++;
			if( cmin >= 60)			/* Minutes no greater than 60 */
			{
				cmin = 0;
				chour++;
				if( chour >= 23 )	/* Hours no greater than 23 */
				{
					chour = 0;
				}					
			}
			/* print("%d:%d:%d", chour,cmin,csec); */
			checkAlarm();
		}
		
		if(tInactive > 15 && tInactive < 18)
		{
			if(dim > 0)
				tdim = dim;
			setDim(-1);
		}
    }/* while */
	                              
    return(0);
}

/*! \brief Check current time vs alarm times
 * Checks the hour of an alarm. If true, it then checks the minutes */
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

/*! \brief Set current Dim Value
 * Provides a central function to call in order to change the value.
 * NOTE: This <b>does</b> change the brightness level
 * \param arg The new brightness level
 * \return Whether the move has been performed */
void setDim( int arg )
{
	dim = arg;
	#if DEBUG
	print("Dim set to %d", dim);
	#endif
}

/*! \brief Set current Blinds value
 * Provides a central function to call in order to change the value.
 * NOTE: Does <b>not</b> change the blinds level
 * \param arg Percent difference that blinds must be set to
 * \return Whether the move has been performed */
void setBlinds( int arg )
{
	blinds  = arg;
	
	#if DEBUG
	print("Blinds set to %d", blinds);
	#endif
}

/*! \brief Modified atoi
 * Parses the command string for integer values
 * \param cmd Command string
 * \param i Length of the entire command string
 * \return Value parsed */
uint8_t getDigits(char * cmd, uint8_t * i)
{
	*i = *i +2;
	uint8_t tmp = (cmd[*i]-48);
	if(cmd[*i+1] != '\0' && cmd[*i+1] != ':')
	{
		*i = *i + 1;
		tmp *= 10;				/* this is a tens value */
		tmp += (cmd[*i]-48);	/* add ones */
	}		
	return tmp;	
}

/*! \brief Monitor any movement registered by PIR sensor
 * In the even of motion, the PIR data line will be
 * high. The line is debounced in order to not receive noise. */
void checkPIR()
{
	if( !active && !(PINB & (1 << PINB0)))
	{
		_delay_ms(25); /* debouncing of PIR line */
		if( !(PINB & (1 << PINB0)) && !active)
		{
			#if DEBUG
			print("Motion!!");
			#endif
			active = 1;
		
			tInactive = 0;
			if(dim != tdim)
				setDim(tdim);
		}			
	}
	else if(active && (PINB & (1<<PINB0)))
	{
		active = 0;
		
	}	
}

/*! \brief Initializes AC light control
 * Declares timer settings and starts the timer
 * Initializes the interrupt values specific to the phase control
 * \param dutycycle Duty cycle of AC wave */
void initAC(int dutycycle)
{
	DDRC |= (1<<DDC0);
	
    OCR0A = 130;				/* cap of Timer0 */
    OCR0B = dutycycle;
                
    /* Timer */
    TCCR0A |= (1 << WGM01);     /* set CTC (Clear Timer on Compare) Mode */
    TCCR0B |= (1 << CS01);      /* set prescaler to 8 and starts PWM */
    TIMSK0 = (1 << OCIE0A) | (1 << TOIE0);    /* Enable OVF */
	
	/* Interrupt */
    EICRA = (1 << ISC01);
    EIMSK |= (1 << INT0);
}

/*! \brief Initializes Light to Frequency Converter
 *  Sets the proper interrupt values for PCINT20.
 *	This uses the same timer as the PIR sensor */
void initL2F()
{
	PCICR |=  (1 << PCIE2);
	PCMSK2 |= (1 << PCINT20);	
}

/*! \brief Initialize Personal Infrared Sensor
 *  Sets the proper interrupt values for PCINT0 */
void initPIR()
{
	PORTB |= (1 << PORTB0);
	/* Sets a 1Hz Timer */
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 0xF423;
}

/*! \brief Initialize Servos that control Blinds
 *  A nice hack that uses a smaller, 8-bit timer to do the work usually
 *  relegated to a 16-bit timer */
void initServo() 
{
	DDRC |= (1 << PORTC2);
	TCNT2 = 0;
	
	
	
	TCCR2A = 0;		/**< Prescalar /1024 */
	OCR2A = 1;
	TIMSK2 = (1 << OCIE2A) | (1 << TOIE2);  /**< Enable OVF */
} 

/*! \brief Open/Close Blinds
 * Calculates the amount of time to open or close the blinds.
 * Sign value of the percentage decides servo direction.
 * Positive percentage moves forward, negative backward.
 * \param percent Amount to open/close blinds */
void static varyBlinds(int8_t percent) 
{
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

/*! \brief Light to Frequency Detection
 * If the L2F sensor has pulsed high, we increment the NumChanges variable. */
ISR(PCINT2_vect)
{
	if(PIND & (1<< PORTD4))
		NumChanges++;
	
}

/*! \brief Bluetooth Receive
 * Receives a string from the Bluetooth module.
 * Relies on ';' terminator */
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
		print("Byte!");
		#endif
		if( received != 10)
		{
			#if DEBUG
			/* print(" %c at %d", received, i); */
			#endif
			command[i] = received;			
			i++;
			while ( !(UCSR0A & (1<<RXC0)) );
			received = UDR0;
		}
	}
	command[i] = '\0';
	set = 0;
} 
 
/*! \brief AC Lighting Control
 * CREATED: 2/15/2012 */
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

/*! \brief 1Hz Timer
 * Every second, the microcontroller performs the Light to Frequency calculation.
 * If the PIR sensor hasn't sensed any movement, the tInactive variable is also incremented. */
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

/*! \brief Timer 2 Compare 
 *	When the timer has achieved the proper PWM value, set the 
 *	Servo data line low */
ISR(TIMER2_COMPA_vect)
{
	if(sActive)
		PORTC &= ~(1 << PORTC2);
}

/*! \brief Timer 2 Overflow for Servo Control
 * Sets the Servo data pin high when timer has reverted to 0 */
ISR(TIMER2_OVF_vect)
{
	if(sActive)
		PORTC |= (1 << PORTC2);
}

/*! \brief AC - Zero Cross Detection */
ISR(INT0_vect)
{
    zerocross = 1;
}

/*! \brief Prevents Microcontroller from resetting */
ISR(__vector_default){}

/*! \brief Allows for dynamic delay
 * \param tick Custom milliseconds to delay */
void static inline delay_ms(uint16_t tick)
{
	for(int i = 0; i < tick; ++i)
		_delay_ms(1);
}


/*! \mainpage Welcome to Project LASAR!
 *
 * \section team_mem Team Members
 *
 * Chris Williams, Deron Jakub, John Laughlin, Vincent Lau
 *
 * \section abstract Abstract
 *
 * The Light Automation System Accessed Remotely, or LASAR, will allow residents to automate
 * and centrally control the light fixtures and window blinds of a home environment. A remote
 * device will connect and control several satellite units in different rooms, each connected to the
 * light fixtures and window blinds in the room. Sensors built into the satellites will allow the
 * system to monitor current light levels and motion in each room.
 *
 * The LASAR will replace the current light automation systems and light controls in a household.
 * User-determined settings will be used to determine the desired brightness of the rooms at
 * different times of day via the light sensor, along with preferences between artificial and ambient
 * light. A motion sensor will shut off the artificial lights in an unoccupied room to prevent wasted
 * energy. The LASAR will empower a user with total lighting control of a household while
 * minimizing energy wasted on inappropriate lighting.
 */