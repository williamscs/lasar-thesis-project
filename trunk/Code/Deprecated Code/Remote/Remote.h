/*
 * Remote.h
 *
 * Created: 3/27/2012 8:18:29 PM
 *  Author: Dubs
 */ 


#ifndef REMOTE_H_
#define REMOTE_H_

#define BIT16

#define DLY	\
asm ("NOP"); \
asm ("NOP");

#define M68_16	0
#define M68_8	1
#define M80_16	2
#define M80_8	3
#define MSPI_3	0xE
#define MSPI_4	0xF

// color definitions
#define	BLACK	0x0000
#define	BLUE	0x001F
#define	RED 	0xF800
#define	GREEN	0x07E0
#define CYAN	0x07FF
#define MAGENTA	0xF81F
#define YELLOW	0xFFE0
#define WHITE	0xFFFF
// maximum values for R, G, B are 31, 63, 31 (5b, 6b, 5b)
// 16 bit color RRRR RGGG GGGB BBBB
#define RGB(r,g,b)	(WORD)(((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F))

//Function Prototypes
void init10msTimer();
void display_rgb(unsigned short data);
void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );



#endif /* REMOTE_H_ */