/*
 * MEMIF.c
 *
 * Created: 29/04/2020 00:17:25
 *  Author: sony
 */ 
#include "std_types.h"

#include "MEM_IF.h"

#include "MEM_IF_Cfg.h"

#include "EEPROM_EXT.h"

#include "EEPROM_INT.h"


////
#define MEM_IF_BUSY		0 
#define MEM_IF_WRITE_DONE 		1u  
#define MEM_IF_READ_DONE 		1u

#define EEPROM_MODULE_NUM	2

/*  **************************               Typedef 					*********************				*/

typedef struct {
	uint8_t BlockIndex;
	uint8_t OperationId;
	uint8_t* DataPtr;
	
} str_Buffer_t;

typedef EEEXT_CheckType(*WritePtr[EEPROM_MODULE_NUM])(unsigned char StartAddress, const unsigned char* DataPtr, unsigned char Length);
typedef EEEXT_CheckType(*WritePtrType)(unsigned char StartAddress, const unsigned char* DataPtr, unsigned char Length);
typedef EEEXT_CheckType(*ReadPtr[EEPROM_MODULE_NUM])(unsigned char StartAddress,unsigned char* DataPtr,unsigned char Length);
typedef EEEXT_CheckType(*ReadPtrType)(unsigned char StartAddress,unsigned char* DataPtr,unsigned char Length);


/*  **************************               Define 					*********************				*/

#define STATE_IDLE					0u
#define STATE_WRITING 				1u
#define STATE_READING				2u
#define STATE_WATING_WRITE			3u
#define STATE_WATING_READ			4u
#define BUFERR_LOCKED				1u
#define BUFERR_UNLOCKED				0u
#define BLOCK_INDEX_INIT_VALUE		0u	 
#define  OPERATION_INIT_VALUE		0u
#define EEINT_WRITE_FUN_INDEX		0u 
#define EEEXT_WRITE_FUN_INDEX		1u
#define EEINT_READ_FUN_INDEX		0u 
#define EEEXT_READ_FUN_INDEX		1u
#define WRITE_FLAG_CLEAR_VALUE		0u 
#define READ_FLAG_CLEAR_VALUE		0u
#define WRITE_FLAG_SET_VALUE		1u 
#define READ_FLAG_SET_VALUE			1u
#define INIT_MEM_IF_INDEX			0u
/*  **************************               globale variable 					*********************				*/

static uint8_t gu8_BufferLock  ;

static  str_Buffer_t gstr_DataBuffer ; 
 
static uint8_t gu8_State ;
 
WritePtr ga_WritePtrToFun ;
ReadPtr ga_ReadPtrToFun ;
// static uint32_t ga_WriteDoneCbf[] ;
// static uint32_t ga_ReadDoneCbf[] ;
 static uint8_t DateWriteFlag;
 static uint8_t DataReadFlag;

void MEMIF_Init(void){
	gu8_State = STATE_IDLE;
	gu8_BufferLock = BUFERR_UNLOCKED;
	gstr_DataBuffer.BlockIndex=BLOCK_INDEX_INIT_VALUE ; 
	gstr_DataBuffer.OperationId= OPERATION_INIT_VALUE;	
	gstr_DataBuffer.DataPtr =NULL;						
	ga_WritePtrToFun[EEINT_WRITE_FUN_INDEX]= (WritePtrType)EEINT_ReqWrite; 
	//ga_WritePtrToFun[EEEXT_WRITE_FUN_INDEX]=EEEXT_ReqWrite;  
	ga_ReadPtrToFun[EEINT_READ_FUN_INDEX]=(ReadPtrType) EEINT_ReqRead;     
	//ga_ReadPtrToFun[EEEXT_READ_FUN_INDEX]= EEEXT_ReqRead;
	
	DateWriteFlag =WRITE_FLAG_CLEAR_VALUE;
	DataReadFlag = READ_FLAG_CLEAR_VALUE; 
						
}

MEMIF_CheckType MEMIF_ReqWriteBlock(unsigned char BlockId,const unsigned char* DataPtr){
	MEMIF_CheckType ErrorStatus = MEMIF_OK;
	uint8_t u8_MEM_IF_Index =INIT_MEM_IF_INDEX;
	if (gu8_BufferLock == BUFERR_UNLOCKED)
	{
		
		if(DataPtr != NULL)
		{


			for(;u8_MEM_IF_Index < MEMIF_NUM_OF_BLOCKS; u8_MEM_IF_Index++)
			{
				if(BlockId == MEMIF_ConfigParam.BlockConfigPtr[u8_MEM_IF_Index].LogicalBlockId  )
				{
				gstr_DataBuffer.BlockIndex = u8_MEM_IF_Index  ;
				gstr_DataBuffer.DataPtr = (unsigned char *)DataPtr ;
				
				gu8_State = STATE_WRITING;
				gu8_BufferLock = BUFERR_LOCKED;
					
				}
				else 
				{
					
				}
			}
			if(u8_MEM_IF_Index == MEMIF_NUM_OF_BLOCKS)
			{
				ErrorStatus = MEMIF_NOK;
			}
			else
			{
				
			}
		}
		
		else 
		{
			ErrorStatus = MEMIF_NOK;
		}
		
		
	}
	else
	{
		ErrorStatus = MEMIF_BUSY ;
	}
	return ErrorStatus ;
}

MEMIF_CheckType MEMIF_ReqReadBlock(unsigned char BlockId, unsigned char* DataPtr){

	MEMIF_CheckType ErrorStatus = MEMIF_OK;
	uint8_t u8_MEM_IF_Index =INIT_MEM_IF_INDEX;
	if (gu8_BufferLock == BUFERR_UNLOCKED)
	{
		if(DataPtr != NULL)
		{
			for(;u8_MEM_IF_Index < MEMIF_NUM_OF_BLOCKS; u8_MEM_IF_Index++)
			{
				if(BlockId == MEMIF_ConfigParam.BlockConfigPtr[u8_MEM_IF_Index].LogicalBlockId  )
				{
				gstr_DataBuffer.BlockIndex = u8_MEM_IF_Index  ;
				gstr_DataBuffer.DataPtr = DataPtr ;
				
				gu8_State = STATE_READING;
				gu8_BufferLock = BUFERR_LOCKED;
					
				}
				else 
				{
					
				}
			}
			if(u8_MEM_IF_Index == MEMIF_NUM_OF_BLOCKS)
			{
				ErrorStatus = MEMIF_NOK;
			}
			else
			{
				
			}
		}
		
		else 
		{
			ErrorStatus = MEMIF_NOK;
		}
		
		
	}
	else
	{
		ErrorStatus = MEMIF_BUSY ;
	}
	
	return ErrorStatus ;
}

void MEMIF_Main(void){
	switch (gu8_State){
		case STATE_IDLE :
		break;
		
		case STATE_WRITING :;
			
			uint8_t BlockLength_write ;
			uint16_t BlockPhyAddress_write;
			gstr_DataBuffer.OperationId = MEMIF_ConfigParam.BlockConfigPtr[gstr_DataBuffer.BlockIndex].BlockStoragePlace; 
			BlockPhyAddress_write = MEMIF_ConfigParam.BlockConfigPtr[gstr_DataBuffer.BlockIndex].BlockPhyAddress; 
			BlockLength_write = MEMIF_ConfigParam.BlockConfigPtr[gstr_DataBuffer.BlockIndex].BlockLength ;
			ga_WritePtrToFun[gstr_DataBuffer.OperationId](BlockPhyAddress_write,(gstr_DataBuffer.DataPtr),BlockLength_write);
			
			gu8_State = STATE_WATING_WRITE;
		
		
		break;
		case STATE_READING:;
			
			uint8_t BlockLength ;
			uint16_t BlockPhyAddress;
			
			gstr_DataBuffer.OperationId = MEMIF_ConfigParam.BlockConfigPtr[gstr_DataBuffer.BlockIndex].BlockStoragePlace; 
			BlockPhyAddress = MEMIF_ConfigParam.BlockConfigPtr[gstr_DataBuffer.BlockIndex].BlockPhyAddress; 
			BlockLength = MEMIF_ConfigParam.BlockConfigPtr[gstr_DataBuffer.BlockIndex].BlockLength ;
			ga_ReadPtrToFun[gstr_DataBuffer.OperationId](BlockPhyAddress,gstr_DataBuffer.DataPtr,BlockLength);
			gu8_State = STATE_WATING_READ;
		break;
		case STATE_WATING_WRITE :
			if(DateWriteFlag == WRITE_FLAG_SET_VALUE)
			{
				DateWriteFlag = WRITE_FLAG_CLEAR_VALUE ;
				gu8_State = STATE_IDLE ;
				gu8_BufferLock = BUFERR_UNLOCKED;
				MEMIF_ConfigParam.BlockWriteDoneNotif(); 

			}
			else 
			{
				
			}
		
		break;
		case STATE_WATING_READ :
			if(DataReadFlag == READ_FLAG_SET_VALUE)
			{
				DataReadFlag = READ_FLAG_CLEAR_VALUE ;
				gu8_State = STATE_IDLE ;
				gu8_BufferLock = BUFERR_UNLOCKED;
				MEMIF_ConfigParam.BlockReadDoneNotif(); 
			}
			else 
			{
				
			}
		break;
		
	}
	
}

void MEMIF_IntEepromWriteCbk(void){
	DateWriteFlag = 1;
}

void MEMIF_IntEepromReadCbk(void){
	 DataReadFlag = 1;
}

void MEMIF_ExtEepromWriteCbk(void){
		DateWriteFlag = 1;
}

void MEMIF_ExtEepromReadCbk(void){
	 DataReadFlag = 1 ;
}

