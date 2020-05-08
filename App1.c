/*
 * App1.c
 *
 * Created: 29/04/2020 20:30:28
 *  Author: sony
 */ 
#include "Btn.h"
#include "NVM.h"
#include "lcd_4bit.h"
#include <stdlib.h>

#define BTN1_ID 0x00
#define COUNTER1_BLOCK_ID 0x00
#define DISPLAY_RAW 0x00

#define STATE_IDLE         0x00
#define STATE_READING      0x01
#define STATE_WRITING      0x02



static unsigned char State;
static unsigned char DateWriteFlag;
static unsigned char DataReadFlag;
void App1_init(void)
{
	State = STATE_IDLE;
	DateWriteFlag = (unsigned char)0;
	DataReadFlag = (unsigned char)0;
}
void App1_WriteCbk(void)
{
	DateWriteFlag = (unsigned char)1;
}
void App1_ReadCbk(void)
{
	DataReadFlag = (unsigned char)1;
}
void App1_main(void)
{
	static unsigned char CounterData;
	switch(State)
	{
		case STATE_IDLE:
		{
			BtnStateType BtnState;
			BTN_GetState(&BtnState,BTN1_ID);
			if(BtnState == BUT_PRSSED)
			{
				NVM_ReadBlock(COUNTER1_BLOCK_ID,&CounterData);
				State = STATE_READING;
			}
			else
			{
				
			}
		}
		break;
		case STATE_READING:
		{
			if(DataReadFlag == 1)
			{
				char str[4];
				DataReadFlag = 0;
				CounterData ++;
				lcd_gotoxy(DISPLAY_RAW,0);
				lcd_dispString("    ");
				lcd_gotoxy(DISPLAY_RAW,0);
				itoa(CounterData,str,10);
				lcd_dispString(str);	
				NVM_WriteBlock(COUNTER1_BLOCK_ID,&CounterData);
				State = STATE_WRITING;				
			}
			else
			{
				
			}
		}
		break;
		case STATE_WRITING:
		{
			if(DateWriteFlag == 1)
			{
				DateWriteFlag = 0;
				State = STATE_IDLE;
			}
		}
		break;
	}
}