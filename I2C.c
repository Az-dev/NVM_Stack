/*
 * I2C.c
 *
 * Created: 28/04/2020 23:20:59
 *  Author: sony
 */ 
 /*
#include <avr/io.h>
#include "BIT_MATH.h"
#include <math.h>

#define TRUE  1
#define FALSE 0

#define START				0
#define START_WAITING		1
#define SEND_SLA_W			2 
#define SEND_SLA_W_WAITING	3
#define SEND_SLA_R			2
#define SEND_SLA_R_WAITING	3
#define SEND_BYTE			4
#define SEND_BYTE_COMPLETE	5
#define READ_BYTE			4
#define READ_BYTE_COMPLETE	5

unsigned char gu8_I2C_StopFlag;
void I2C_Init(void)
{
	double POW = 0;
	TWCR = (1<<TWEA) | (1<<TWEN);
	gu8_I2C_StopFlag = TRUE;
	
	switch (I2C_ConfigType.PreScaler)
	{
		case 1:
		CLR_BIT(TWSR,1);
		CLR_BIT(TWSR,0);
		break;
		
		case 4:
		CLR_BIT(TWSR,1);
		SET_BIT(TWSR,0);
		break;
		
		case 16:
		SET_BIT(TWSR,1);
		CLR_BIT(TWSR,0);
		
		case 64:
		SET_BIT(TWSR,1);
		SET_BIT(TWSR,0);
		
	}
	POW = pow(8,I2C_ConfigType.PreScaler)
	TWBR = ((I2C_ConfigType.CPUFreq/I2C_ConfigType.TargerRate)-16)/POW;
	
}
I2C_CheckType I2C_ReqWrite(unsigned char SlaveAddress, const unsigned char* DataPtr, unsigned char DataLen)
{
	static unsigned char u8_I2C_State = START;
	static unsigned char u8_DataIndex = 0;
	static unsigned char u8_GuardFlag = FALSE;
	I2C_CheckType enm_I2C_Check = I2C_NOK;
	if (gu8_I2C_StopFlag || u8_GuardFlag)
	{
		gu8_I2C_StopFlag = FALSE;
		u8_GuardFlag = TRUE;
		switch (u8_I2C_State)
		{
			case START:
			TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN);
			u8_I2C_State = START_WAITING;
			break;
			
			case START_WAITING:
			if (!(TWCR & (1<<TWINT)))
			{
				u8_I2C_State = SEND_SLA_W;
			}
			break;
			
			case SEND_SLA_W:
			TWDR = (SlaveAddress<<1);
			TWCR = (1<<TWINT) | (1<<TWEN);
			u8_I2C_State = SEND_SLA_W_WAITING;
			break;
			
			case SEND_SLA_W_WAITING:
			if (!(TWCR & (1<<TWINT))) 
			{
				u8_I2C_State = SEND_BYTE;
			}
			break;
			
			case SEND_BYTE:
			TWDR = DataPtr[u8_DataIndex]; 
			TWCR = (1<<TWINT) | (1<<TWEN);
			u8_DataIndex++;
			u8_I2C_State = SEND_BYTE_COMPLETE;
			break;
			
			case SEND_BYTE_COMPLETE:
			if (!(TWCR & (1<<TWINT)))
			{
				if (u8_DataIndex < DataLen)
				{
					u8_I2C_State = SEND_BYTE;
				}
				else
				{
					u8_GuardFlag = FALSE;
					enm_I2C_Check = I2C_OK;
					I2C_ConfigType.ActionDoneCbkPtr();
				}
			}
			break;
			
		}
	}
	else
	{
		enm_I2C_Check = I2C_NOK;
	}
	return enm_I2C_Check;
}

I2C_CheckType I2C_ReqRead(unsigned char SlaveAddress, unsigned char* DataPtr, unsigned char DataLen)
{
	static unsigned char u8_I2C_State = START;
	static unsigned char u8_DataIndex = 0;
	static unsigned char u8_GuardFlag = FALSE;
	I2C_CheckType enm_I2C_Check = I2C_NOK;
	if (gu8_I2C_StopFlag || u8_GuardFlag)
	{
		switch (u8_I2C_State)
		{
			case START:
			TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN);
			u8_I2C_State = START_WAITING;
			break;
			
			case START_WAITING:
			if (!(TWCR & (1<<TWINT)))
			{
				u8_I2C_State = SEND_SLA_W;
			}
			break;
			
			case SEND_SLA_R:
			TWDR = ((SlaveAddress<<1)|1);
			TWCR = (1<<TWINT) | (1<<TWEN);
			u8_I2C_State = SEND_SLA_R_WAITING;
			break;
			
			case SEND_SLA_R_WAITING:
			if (!(TWCR & (1<<TWINT)))
			{
				u8_I2C_State = READ_BYTE;
			}
			break;
			
			case READ_BYTE:
			DataPtr[u8_DataIndex] = TWDR;
			TWCR = (1<<TWINT) | (1<<TWEN);
			u8_DataIndex++;
			u8_I2C_State = READ_BYTE_COMPLETE;
			break;
			
			case READ_BYTE_COMPLETE:
			if (!(TWCR & (1<<TWINT)))
			{
				if (u8_DataIndex < DataLen)
				{
					u8_I2C_State = READ_BYTE;
				}
				else
				{
					u8_GuardFlag = FALSE;
					I2C_ConfigType.ActionDoneCbkPtr();
					enm_I2C_Check = I2C_OK;
				}
			}
			break;
			
		}
	}
	else
	{
		enm_I2C_Check = I2C_NOK;
	}
	return enm_I2C_Check;
}

void I2C_Stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
	gu8_I2C_StopFlag = TRUE;
}
*/