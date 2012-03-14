/*
 * Satellite.h
 *
 * Created: 3/13/2012 4:10:24 PM
 *  Author: Chris Williams
 */ 


#ifndef SATELLITE_H_
#define SATELLITE_H_


//Function headers
void static inline delay_ms(uint16_t tick);
void initTimer( int dutycycle);
void initInterrupts();
void setCycle(int dutycycle);
void initServo(const int period_64us);
void openWindow(int time);	//inline ensures function execution time optimization
void closeWindow(int time);
void SPI_init(void);
char SPI_Transmit(char cData);
void getTime(uint8_t * hours, uint8_t * minutes, uint8_t * seconds);




//RTC Registers
//Read Registers
#define SECONDS_R	0x00
#define MINUTES_R	0x01
#define HOURS_R		0x02

//Write Registers
#define SECONDS_W	0x80
#define MINUTES_W	0x81
#define HOURS_W		0x82

//Control
#define CTRL		0x0E
#define CTRL_STATUS	0x0F





#endif /* SATELLITE_H_ */