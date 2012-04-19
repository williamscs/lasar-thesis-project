/*
 * Satellite.h
 *
 * Created: 3/13/2012 4:10:24 PM
 *  Author: Chris Williams
 */ 


#ifndef SATELLITE_H_
#define SATELLITE_H_

#define DEBUG 0

//Servo Constants
#define SERVO_PERIOD	312
#define SERVO_FWD 0x0A  //1ms
//#define SERVO_CEN 23	//1.5ms (not needed)
#define SERVO_REV 0x2F	//2ms
#define CLOSE_TIME 7000

 typedef struct
{
    uint8_t hour;
    uint8_t min;
	uint8_t dim;
	uint8_t blinds;
} Pref;

 typedef struct
{
    uint8_t hour;
    uint8_t min;
} Time;




//Function headers
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
void static inline varyBlinds(int8_t percent);	//inline ensures function execution time optimization
void SPI_init(void);
char SPI_Transmit(char cData);
void getTime(uint8_t * hours, uint8_t * minutes, uint8_t * seconds);
uint8_t getDigits(char * cmd, uint8_t * i);




//AC Values
#define MAX 25
#define OFF -1




#endif /* SATELLITE_H_ */