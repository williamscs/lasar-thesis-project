/*
 * USART.h
 *
 * Created: 2/2/2012 2:22:00 PM
 *  Author: Chris Williams
 */ 


#ifndef USART_H_
#define USART_H_


//USART Stuff
#define FOSC F_CPU    // Clock Speed
#define BAUD 38400ul
#define MYUBRR 0x0019//((F_CPU/(BAUD<<4))-1)

void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void USART_Print(char * input, int n);
int print(char *format, ...);


#endif /* USART_H_ */
