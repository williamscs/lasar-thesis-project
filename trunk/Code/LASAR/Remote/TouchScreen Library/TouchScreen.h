/*
 * TouchScreen.h
 *
 * Created: 2/22/2012 4:28:05 PM
 *  Author: Dubs
 */ 


#ifndef TOUCHSCREEN_H_
#define TOUCHSCREEN_H_

/*Pin Definitions
 * These can be assumed to be the same values as the port,
 * meaning that ADC0 is defined as the same value as PORTF0
 */
#define TS_DIR DDRF
#define TS_PORT PORTF
#define X1 PORTF0
#define Y2 PORTF1
#define X2 PORTF2
#define Y1 PORTF3



void initTouchScreen(void);
uint16_t getX();
uint16_t getY();
uint16_t analogRead( uint8_t pin);



#endif /* TOUCHSCREEN_H_ */