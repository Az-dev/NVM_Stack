/*
* NVM.h
*
* Created: 29/04/2020 00:23:34
*  Author: sony
*/

#ifndef NVM_H_
#define NVM_H_

/*--- NVM state machine states ----*/
#define NVM_IDLE 0u
#define NVM_ERROR 1u
#define NVM_BLOCK_WRITE 2u
#define NVM_BLOCK_READ 3u
#define NVM_CHUNK_WRITE 4u
#define NVM_CHUNK_READ 5u

/*--- Flags states ---*/
#define NVM_WRITE_BLOCK_WAIT 0u
#define NVM_READ_BLOCK_WAIT 1u
#define NVM_WRITE_BLOCK_DONE 2u
#define NVM_READ_BLOCK_DONE 3u


typedef enum
{
   NVM_OK,
   NVM_NOK,
   NVM_BUSY
} NVM_CheckType;
typedef void (*NvmActionDoneCbkPtrType)(void);
typedef struct
{
   unsigned char BlockId;
   unsigned char BlockLength;
   unsigned char *BlockRamAddress;
   NvmActionDoneCbkPtrType BlockWriteNotifPtr;
   NvmActionDoneCbkPtrType BlockReadNotifPtr;
} NVM_ConfigType;
typedef struct
{
   NvmActionDoneCbkPtrType WriteAllCallBack;
   NvmActionDoneCbkPtrType ReadAllCallback;
} NVM_GlobalCallbacksType;
void NVM_Init(void);
NVM_CheckType NVM_ReadAll(void);
NVM_CheckType NVM_WriteAll(void);
NVM_CheckType NVM_ReadBlock(unsigned char BlockId, unsigned char *DataPtr);
NVM_CheckType NVM_WriteBlock(unsigned char BlockId, const unsigned char *DataPtr);
void NVM_Main(void);
void NVM_WriteBlockDoneNotif(void); /* for interacting with memory interface */
void NVM_ReadBlockDoneNotif(void);  /* for interacting with memory interface */

#endif /* NVM_H_ */