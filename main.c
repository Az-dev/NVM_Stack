/*
* Memory_interface_app.c
*
* Created: 28/04/2020 20:44:32
* Author : sony
*/

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "DIO.h"
#include "lcd_4bit.h"
#include "Btn.h"
#include "I2C.h"
#include "EEPROM_EXT.h"
#include "EEPROM_INT.h"
#include "MEM_IF.h"
#include "NVM.h"
#include "App1.h"
#include "App2.h"
#define STATE_IDLE    0x00
#define STATE_READING 0x01
#define STATE_RUNNING 0x02
#define STATE_WRITING 0x03

static unsigned char SystemState;
static unsigned char ReadAllDone;
static unsigned char WriteAllDone;
void Main_Init(void);
void Main_Main(void);
void Main_WriteAllDone(void)
{
	WriteAllDone = 1;
}
void Main_ReadAllDone(void)
{
	ReadAllDone = 1;
}
void task1_code(void *p)
{

	while(1)
	{
		vTaskDelay(50);
		EEINT_Main();
		//EEEXT_Main();
		MEMIF_Main();
		NVM_Main();
		Main_Main();
		
	}
}
int main(void)
{
	DIO_Init();
	//I2C_Init();
	EEINT_Init();
	//EEEXT_Init();
	MEMIF_Init();
	NVM_Init();
	App1_init();
	App2_init();
	lcd_init();
	BTN_Init();
	
	/*Creat tasks*/
	xTaskCreate(task1_code,NULL,200,NULL,2,NULL);
	vTaskStartScheduler();
	/* Replace with your application code */
	while (1)
	{
		
	}
}

void Main_Init(void)
{
	SystemState = STATE_IDLE;
	ReadAllDone = 0;
	WriteAllDone = 0;
	
}
void Main_Main(void)
{
	BtnStateType BtnState;
	switch(SystemState)
	{
		case STATE_IDLE:
		{
			lcd_clrScreen();
			lcd_dispString("Reading NVM");
			NVM_ReadAll();
			SystemState = STATE_READING;
		}
		break;
		case STATE_READING:
		{
			if(ReadAllDone == 1)
			{
				ReadAllDone = 0;
				lcd_clrScreen();
				lcd_dispString("Waiting for the first Press");
				SystemState = STATE_RUNNING;
			}
		}
		break;
		case STATE_RUNNING:
		{
			BTN_Manager();
			App1_main();
			App2_main();
			BTN_GetState(&BtnState,0x02);
			if(BtnState == BUT_PRSSED)
			{
				lcd_clrScreen();
				lcd_dispString("Writing NVM");
				NVM_WriteAll();
				SystemState = STATE_WRITING;
			}
			
		}
		break;
		case STATE_WRITING:
		{
			if(WriteAllDone == 1)
			{
				WriteAllDone = 0;
				lcd_clrScreen();
				lcd_dispString("Write all done");
				lcd_gotoxy(0,1);
				lcd_dispString("you can power off");
			}
		}
		break;
	}
}