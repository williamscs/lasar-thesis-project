/*
 * Satellite.h
 *
 * Created: 3/13/2012 4:10:24 PM
 *  Author: Chris Williams
 */ 


#ifndef SATELLITE_H_
#define SATELLITE_H_

#define DEBUG 0			/**< Allows for a debug mode that provides more verbose feedback */

/*AC Values */
#define MAX 1			/**< AC Lighting fully on */
#define OFF -1			/**< AC Lighting fully off */

/* Servo Constants */
#define SERVO_PERIOD	312		/**< Period */
#define SERVO_FWD 0x0A  //1ms	/**< PWM signal for moving servo forward (1 ms) */
#define SERVO_REV 0x2F	//2ms	/**< PWM signal for moving servo backward (2ms) */
#define CLOSE_TIME 7000			/**< Amount of time needed to fully open/close the blinds */

/*! \brief Preference type
 *  Holds all values needed for storing preferences
 *  */
 typedef struct
{
    uint8_t hour;	/**< Alarm hour */
    uint8_t min;	/**< Alarm minute */
	uint8_t dim;	/**< Preference brightness */
	uint8_t blinds; /**< Preference blinds level */
} Pref;

/* Function headers */
void static inline delay_ms(uint16_t tick);
void initAC(int dutycycle);
void initL2F();
void initPIR();
void checkAlarm();
void setDim( int arg );
void setBlinds( int arg );
void checkPIR();
void initServo();
void initTimer( int dutycycle);
void static inline varyBlinds(int8_t percent);
void SPI_init(void);
char SPI_Transmit(char cData);
void getTime(uint8_t * hours, uint8_t * minutes, uint8_t * seconds);
uint8_t getDigits(char * cmd, uint8_t * i);

#endif /* SATELLITE_H_ */