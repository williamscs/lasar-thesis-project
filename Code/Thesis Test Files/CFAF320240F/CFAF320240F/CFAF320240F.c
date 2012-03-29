
#define F_CPU 10000000UL

#include<avr/io.h>
//#include"MMC_SD/MMC_SD.h"
//#include"FAT/FAT.h"
#include <util/delay.h>

// control line pins
#define	LCD_CD	PC5   
#define	LCD_WR  PC6
#define	LCD_RD  PC7
#define	LCD_CS	PC4
#define	LCD_RES	PC3

// macros for control lines
#define CLR_CD PORTC &= ~(1<<LCD_CD);
#define SET_CD PORTC |=  (1<<LCD_CD);

#define CLR_CS PORTC &= ~(1<<LCD_CS);
#define SET_CS PORTC |=  (1<<LCD_CS);

#define CLR_RESET PORTC &= ~(1<<LCD_RES);
#define SET_RESET PORTC |=  (1<<LCD_RES);

#define CLR_WR PORTC &= ~(1 << LCD_WR);
#define SET_WR PORTC |=  (1 << LCD_WR);
#define CLR_RD PORTC &= ~(1 << LCD_RD);
#define SET_RD PORTC |=  (1 << LCD_RD);

// color definitions
#define	BLACK	  0x0000
#define	BLUE	  0x001F
#define	RED 	  0xF800
#define	GREEN   0x07E0
#define CYAN	  0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE	  0xFFFF

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

//unsigned char * PATH = "\\batch";

//extern uint16 SectorsPerClust;
//extern uint16 FirstDataSector; 
//extern uint8 FAT32_Enable;
//
//struct FileInfoStruct FileInfo;
//
//struct direntry PictureInfo;
//uint16 totalfiles = 0;

/*************************************************/
void delay(unsigned int t)
{
	unsigned char t1;
	while(t--)
	for(t1=11;t1>0;t1--)
	{asm("nop");}
}
/*************************************************/
void write_command(unsigned int command)
{
	CLR_CS;
	CLR_CD;
	SET_RD;
	SET_WR;

	PORTA=command;
	PORTE=command>>8;

	CLR_WR;
	SET_WR;
	SET_CS;
}

/*************************************************/
void write_data(unsigned int data)
{
	CLR_CS;
	SET_CD;
	SET_RD;
	SET_WR;

	PORTA=data;
	PORTE=data>>8;

	CLR_WR;
	SET_WR;
	SET_CS;
}
/*************************************************/
void initialization()
{
	SET_RD;
	SET_WR;
	SET_CS;
	SET_CD;
	PORTA=0x00;
	PORTE=0x00;

	CLR_RESET;
	delay(200);
	SET_RESET;
	delay(500);

  write_command(0x0028);    // VCOM OTP
  write_data(0x0006);       // Page 55-56 of SSD2119 datasheet

	write_command(0x0000);    // start Oscillator
	write_data(0x0001);       // Page 36 of SSD2119 datasheet

	write_command(0x0010);    // Sleep mode
	write_data(0x0000);       // Page 49 of SSD2119 datasheet

	write_command(0x0001);    // Driver Output Control
	write_data(0x32EF);       // Page 36-39 of SSD2119 datasheet

	write_command(0x0002);    // LCD Driving Waveform Control
	write_data(0x0600);       // Page 40-42 of SSD2119 datasheet

	write_command(0x0003);    // Power Control 1
	write_data(0x6A38);       // Page 43-44 of SSD2119 datasheet

	write_command(0x0011);    // Entry Mode
	write_data(0x6870);       // Page 50-52 of SSD2119 datasheet

	write_command(0X000F);    // Gate Scan Position
	write_data(0x0000);       // Page 49 of SSD2119 datasheet

	write_command(0X000B);    // Frame Cycle Control
	write_data(0x5308);       // Page 45 of SSD2119 datasheet

	write_command(0x000C);    // Power Control 2
	write_data(0x0003);       // Page 47 of SSD2119 datasheet

	write_command(0x000D);    // Power Control 3
	write_data(0x000A);       // Page 48 of SSD2119 datasheet

	write_command(0x000E);    // Power Control 4
	write_data(0x2E00);       // Page 48 of SSD2119 datasheet

	write_command(0x001E);    // Power Control 5
	write_data(0x00BE);       // Page 53 of SSD2119 datasheet

	write_command(0x0025);    // Frame Frequency Control
	write_data(0x8000);       // Page 53 of SSD2119 datasheet

	write_command(0x0026);    // Analog setting
	write_data(0x7800);       // Page 54 of SSD2119 datasheet

	write_command(0x004E);    // Ram Address Set
	write_data(0x0000);       // Page 58 of SSD2119 datasheet

	write_command(0x004F);    // Ram Address Set
	write_data(0x0000);       // Page 58 of SSD2119 datasheet

	write_command(0x0012);    // Sleep mode
	write_data(0x08D9);       // Page 49 of SSD2119 datasheet

	// Gamma Control (R30h to R3Bh) -- Page 56 of SSD2119 datasheet
	write_command(0x0030);
	write_data(0x0000);

	write_command(0x0031);
	write_data(0x0104);

	write_command(0x0032);
	write_data(0x0100);

	write_command(0x0033);
	write_data(0x0305);

	write_command(0x0034);
	write_data(0x0505);

	write_command(0x0035);
	write_data(0x0305);

	write_command(0x0036);
	write_data(0x0707);

	write_command(0x0037);
	write_data(0x0300);

	write_command(0x003A);
	write_data(0x1200);

	write_command(0x003B);
	write_data(0x0800);		 

	write_command(0x0007);    // Display Control 
	write_data(0x0033);       // Page 45 of SSD2119 datasheet

	delay(150);

	write_command(0x0022);    // RAM data write/read
}
/*************************************************/
void Display_Home()
{
	write_command(0x004E);    // RAM address set
	write_data(0x0000);       // Page 58 of SSD2119 datasheet
	write_command(0x004F);    // RAM address set
	write_data(0x0000);       // Page 58 of SSD2119 datasheet

	write_command(0x0044);    // Vertical RAM address position
	write_data(0xEF00);       // Page 57 of SSD2119 datasheet
	write_command(0x0045);    // Horizontal RAM address position 
	write_data(0x0000);       // Page 57 of SSD2119 datasheet
	write_command(0x0046);    // Horizontal RAM address position
	write_data(0x013F);       // Page 57 of SSD2119 datasheet

	write_command(0x0022);    // RAM data write/read
}


void display_rgb(unsigned int data)
{
	unsigned int i,j;
	Display_Home();
	for(i=0;i<320;i++)
  {
  	for(j=0;j<240;j++)
  	{
  		write_data(data);
  	}
  }
}

//void Picture_Slide_Show()
//{
	//uint8 i;              //loop variable
	//uint16 j;             //loop variable
	//uint32 p;             //cluster
	//uint16 *buffer;				//buffer
	//uint16 pics = 1;
	//uint16 slide_show_flag = 1;
	//uint8 sector;
	//uint32 pixels;
//
	//if (totalfiles == 0)
	//return;
//
	//buffer = malloc(512);
	//if (buffer == 0)
	//return;
//
//next: 	//label for "goto"
	//display_rgb(BLACK);
//
	//Search(PATH, &PictureInfo, &pics);                                         //find the file
	//p = PictureInfo.deStartCluster + (((uint32)PictureInfo.deHighClust) << 16);//the first cluster of the file
//
	//sector = 0;
//
	//FAT_LoadPartCluster(p, sector, buffer);	//read a sector
//
//
	//pixels = (uint32) 320 * 240; // total # of pixels to write
//
	//Display_Home();
	//write_command(0x22);
//
	//j = 0; // byte count
	//
	//while (1)
	//{
		//write_data(buffer[j]);  // write 16 bits
		//pixels--;               // which is one pixel
		//j++;                    // word count
//
		//if (pixels == 0)        
		//break;
//
		//if (j == 256)           // time for a new sector     
		//{
			//sector++;
			//if (sector == SectorsPerClust)
			//{
				//p = FAT_NextCluster(p);				// read next cluster
				//sector = 0;
			//}
//
			//FAT_LoadPartCluster(p, sector, buffer);	// read a sector
			//j = 0;
		//}
	//}
//
	//if (slide_show_flag)
	//{
		//for (i=0;i<30;i++)	// delay for a while
		//delay(0xffff);
//
		//pics++;					// increment picture number
		//if (pics > totalfiles)	// if last
		//pics = 1;			// wrap around
//
		//goto next;				// go show next pic
	//}
	//free(buffer);
//}
/*************************************************/
void LCD_test()
{
	unsigned int i,j;
	Display_Home();

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			if(i>279)write_data(BLACK);
			else if(i>239)write_data(BLUE);
			else if(i>199)write_data(GREEN);
			else if(i>159)write_data(CYAN);
			else if(i>119)write_data(RED);
			else if(i>79)write_data(MAGENTA);
			else if(i>39)write_data(YELLOW);
			else write_data(WHITE);
		}
	}
}

/*************************************************/
int main()
{
	//PORTD = 0xF8;	    // pull-up on switches, all others off
	//DDRD  = 0x0F;   	// VPP and led pins output

	DDRA = 0xFF;    	// low data port, all outputs
	DDRE = 0xFF;    	// low data port, all outputs
	
	//PORTC = 0xFF;	    // all pins high
	DDRC  = 0xFF;	    // all outputs

	//uint8 retry = 0;

	//OSCCAL = 0x00;    //in order to operate some low speed card the initialization should run at lowest speed

	delay(0xffff);

	//MMC_SD_Init();    //SPI initialize

	// Initialize the display
	initialization();

	LCD_test();
	_delay_ms(1000);
	//sd_restart: 	//label for "goto"

	//while (MMC_SD_Reset())  // Initialize SD Card
	//{
		//retry++;
		//if (retry > 254)
		//{
			//while (1)
			//{
				//goto sd_restart;				// try again
			//}
		//}
	//}

	//OSCCAL = 0xff;      //run at high speed for normal operation for SD Card

	delay(0xffff);      

	//if (FAT_Init())   	//initialize file system - FAT16 and FAT32 are supported
	//{
		//while (1)
		//{
			//goto sd_restart;				// try again
		//}
	//}

	//Search(PATH, &PictureInfo, &totalfiles);
//
	//Picture_Slide_Show();
//
	while (1)
	{
		PORTC ^= (1 << PORTC0);
		_delay_ms(100);
	}
	return 0;

}
/*************************************************/
