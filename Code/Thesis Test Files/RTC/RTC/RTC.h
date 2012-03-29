/*
 * RTC.h
 *
 * Created: 3/29/2012 3:38:42 PM
 *  Author: Dubs
 */ 

/*
 * DS3234.h
 *
 * Created: 3/20/2012 5:15:49 PM
 *  Author: Dubs
 */ 

#include "SPI\spi.h"

#ifndef RTC_H_
#define RTC_H_

void RTC_init();
void SetTimeDate(int d, int mo, int y, int h, int mi, int s);
char* ReadTimeDate();


#endif /* RTC_H_ */