/*
 * Servo.h
 *
 * Created: 2/29/2012 3:52:41 PM
 *  Author: Dubs
 */ 


#ifndef SERVO_H_
#define SERVO_H_

//SERVO Stuff
#define SERVO_PERIOD	312
#define SERVO_FWD		31  //1ms
#define SERVO_CEN		23	//1.5ms
#define SERVO_REV		16	//2ms

void initServo(const int period_64us);
void set_servo1(int pwm);


#endif /* SERVO_H_ */