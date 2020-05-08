/**
* @file LCD_Driver_4bit_MODE.c
* @brief 4 bit mode lcd driver for ATMEGA16 or 32 .
* @author Islam Mohamed.
*/

#include"lcd_4bit.h"
#include "LCD_Cfg.h"
#include "DIO.h"
#include "macros.h"



static void WriteNipple(unsigned char Nipple);
static void _delay_us(unsigned long int Count);
/////////////////////////////////////////////////////////////////////
//	lcd enable triggering function to make lcd aware about command or
//   data changes.
////////////////////////////////////////////////////////////////////
void enableTrigger(void)
{



	DIO_Write(LCD_EN,0xff);
	_delay_us(2);
	DIO_Write(LCD_EN,0x00);
	_delay_us(2);


}
///////////////////////////////////////////////////////////////////////////////////////
// LCD initialization API : set portB from pinB.4 to pinB.7 as o/p for data and commands
//							 and pinB.0 to pinB.2 o/p for control pins
//////////////////////////////////////////////////////////////////////////////////////
void lcd_init(void)
{



	lcd_sendCommand(0x33); // 4-bit mode
	lcd_sendCommand (0x32);
	lcd_sendCommand (0x28);



	lcd_sendCommand(0x0c); // turn on lcd

	
}


void lcd_sendCommand(unsigned char cmd)
{

	unsigned char high_nibble , low_nibble ;
	
	HIGH_NIBBLE(high_nibble,cmd);
	LOW_NIBBLE(low_nibble,cmd);

	DIO_Write(LCD_RS,0x00);
	DIO_Write(LCD_WR,0x00);

	WriteNipple(high_nibble);
	enableTrigger(); // triggre lcd enable
	WriteNipple(low_nibble);
	enableTrigger();
	_delay_us(100);

}

void lcd_displayChar (unsigned char data)
{

	unsigned char high_nibble , low_nibble ;



	HIGH_NIBBLE(high_nibble,data); // get high nibble data
	LOW_NIBBLE(low_nibble,data); // get low nibble data
	
	DIO_Write(LCD_RS,0xff);
	DIO_Write(LCD_WR,0x00);

	WriteNipple(high_nibble);
	enableTrigger(); // triggre lcd enable

	WriteNipple(low_nibble);
	enableTrigger();
	_delay_us(2);
	

}


void lcd_gotoxy(unsigned char y , unsigned char x )
{
	unsigned char position = 0x80;
	
	switch(y)
	{
		case 0:

		position=position+x;
		
		break;

		case 1:

		position=0xc0;
		position=position+x;

		break;

		case 2:

		position=position+x;

		break;

		case 3:
		position=position+x;
		break;
		
		
	}
	
	lcd_sendCommand(position);


}


void lcd_clrScreen(void)
{

	lcd_sendCommand(0x01);


}


void lcd_dispString(char * ptr)
{

	while(* ptr!= '\0')
	{
		lcd_displayChar(* ptr);
		ptr++;
		
	}



}

void lcd_disp_string_xy(char * ptr , int y , int x)
{

	lcd_gotoxy(y,x);
	lcd_dispString(ptr);

	


}
static void WriteNipple(unsigned char Nipple)
{
	unsigned char Loop;
	unsigned char BitValue;
	unsigned char lut[4] = {LCD_DATA0, LCD_DATA1, LCD_DATA2, LCD_DATA3};
	for(Loop = 0; Loop < 4; Loop++)
	{
		BitValue = (Nipple & (1 << Loop)) >> Loop;
		BitValue = 0 - BitValue;
		DIO_Write(lut[Loop],BitValue);
	}
}

static void _delay_us(unsigned long int Count)
{
 volatile unsigned long int Loop1;
 volatile unsigned long int loop2;
 for(Loop1 = 0; Loop1 < Count; Loop1++)
 {
     for(loop2 = 0; loop2 < 4; loop2++);
 }

}
