/*
 * Servo.c
 *
 * Created: 2/29/2012 3:52:31 PM
 *  Author: Dubs
 */ 
#include<avr\io.h>
#include<avr\sfr_defs.h>
#include<util\delay.h>
#include"Servo.h"

/* 
 * Via http://smrobots.com/node/22 
 * Sets up Timer 1 for fast-PWM operation
 * servos are enabled on B1 and B2
 * Period is given in increments of 64us up to 65535
 * suggest value of 312 for 20ms period
 */
void initServo(const int period_64us) 
{
	//initialize TMR1 (PWM) 
	// clear on compare, fast PWM, TOP=ICR1 (WGM13/WGM12 in TCCR1B)
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	// prescaler 1024 (and WGM12 and WGM13)
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS12); 
	ICR1 = period_64us;
	OCR1A = -1;//off
	OCR1B = -1;//off
	DDRB |= (1 << PORTB1);
} 
 
/* sets "high" time of B1 for pwm*64us
 * 1ms (full reverse) is 16
 * 1.5ms (centered) is 23
 * 2ms (full forward) is 31
 */
void set_servo1(int pwm) 
{
	OCR1A = pwm;
}