/*
 * USART.h
 *
 * Created: 2/2/2012 2:22:00 PM
 *  Author: Chris Williams
 */ 


#ifndef USART_H_
#define USART_H_


//USART Stuff




#endif /* USART_H_ */

void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void print(char * input);