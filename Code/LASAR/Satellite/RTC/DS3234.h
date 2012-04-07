/*
 * DS3234.h
 *
 * Created: 3/20/2012 5:15:49 PM
 *  Author: Dubs
 */ 

#include "SPI\spi.h"

#ifndef DS3234_H_
#define DS3234_H_

void RTC_init();
void SetTimeDate(int d, int mo, int y, int h, int mi, int s);
char* ReadTimeDate();


#endif /* DS3234_H_ */