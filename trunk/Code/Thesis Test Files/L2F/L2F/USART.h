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
#define BAUD 9600UL
#define MYUBRR (F_CPU/(16*BAUD))-1



#endif /* USART_H_ */

void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void USART_Print(char * input);