/*
 * NVM.c
 *
 * Created: 29/04/2020 20:01:42
 *  Author: sony
 */
/*- INCLUDES --------------------------------------------------------------------------------*/
#include "NVM.h"
#include "NVM_Cfg.h"
#include "MEM_IF.h"
#include "std_types.h"
/*- GLOBALS ---------------------------------------------------------------------------------*/
static uint8_t gu8_writeBlockDone;
static uint8_t gu8_readBlockDone;
static uint8_t gu8_NVM_SM_State;
static uint8_t gu8_readBlockCounter;
static uint8_t gu8_writtenBlockCounter;
/*- FUNCTIONS DEFINITIONS -------------------------------------------------------------------*/
/*  
*  Description : Initializes NVM module
*
*  @param void 
*
*  @return void 
*/
void NVM_Init(void)
{
   /* Initialize variable */
   gu8_writeBlockDone = NVM_WRITE_BLOCK_WAIT;
   gu8_readBlockDone = NVM_READ_BLOCK_WAIT;
   gu8_NVM_SM_State = NVM_IDLE;
   gu8_readBlockCounter = 0;
   gu8_writtenBlockCounter = 0;
}

/*
*  Description : reads the entire virtual localized storage and place it inside the temp/mirror storage.
*
*  @param void 
*
*  @return NVM_CheckType 
*/
NVM_CheckType NVM_ReadAll(void)
{
   /* trigger CHUNK_Read state in NVM_Main() */
   gu8_NVM_SM_State = NVM_CHUNK_READ;
   /* Trigger REading the first block */
   MEMIF_ReqReadBlock(NVM_BlocConfig[gu8_readBlockCounter].BlockId, NVM_BlocConfig[gu8_readBlockCounter].BlockRamAddress);   
   /* return success */
   return NVM_OK;
}

/* 
*  Description : copies all the blocks from a temp/mirror storage to a virtual localized storage.
*
*  @param void 
*
*  @return NVM_CheckType
*/
NVM_CheckType NVM_WriteAll(void)
{
   /* trigger CHUNK_Read state in NVM_Main() */
   gu8_NVM_SM_State = NVM_CHUNK_WRITE;
   /* Trigger REading the first block */
   MEMIF_ReqWriteBlock(NVM_BlocConfig[gu8_writtenBlockCounter].BlockId, NVM_BlocConfig[gu8_writtenBlockCounter].BlockRamAddress);
   /* return success */
   return NVM_OK;
}

/*
*  Description : reads one block from the temp/mirror storage and retrieve it back to the user 
*
*  @param unsigned char BlockId
*  @param unsigned char *DataPtr
*
*  @return NVM_CheckType
*/
NVM_CheckType NVM_ReadBlock(unsigned char BlockId, unsigned char *DataPtr)
{
   unsigned char au8_iter = 0u;
   unsigned char au8_blockFound = 0u;
   NVM_CheckType au8_NVM_ReadBlockState = 0u;

   /* search for the specified block specified in configurations array */
   for (; (unsigned char)NVM_NUM_OF_BLOCKS > au8_iter; au8_iter++)
   {
      if (NVM_BlocConfig[au8_iter].BlockId == BlockId)
      {
         unsigned char au8_dataIter = 0u;
         /* raise block found flag */
         au8_blockFound = 1;
         /* loop over the data with-in the block */
         for (; NVM_BlocConfig[au8_iter].BlockLength > au8_dataIter; au8_dataIter++)
         {
            /* read block from mirror into *DataPtr */
            *(DataPtr + au8_dataIter) = *(NVM_BlocConfig[au8_iter].BlockRamAddress + au8_dataIter);
         }
         /* notify upper layer */
         NVM_BlocConfig[au8_iter].BlockReadNotifPtr();
         /* report success */
         au8_NVM_ReadBlockState = NVM_OK;
      }
   }
   if (!au8_blockFound)
   {
      /* report fail */
      au8_NVM_ReadBlockState = NVM_NOK;
   }
   /* report nvm block read state */
   return au8_NVM_ReadBlockState;
}

/*
*  Description : copies one block from user input into temp/mirror storage 
*
*  @param unsigned char BlockId
*  @param const unsigned char *DataPtr
*
*  @return NVM_CheckType
*/
NVM_CheckType NVM_WriteBlock(unsigned char BlockId, const unsigned char *DataPtr)
{
   unsigned char au8_iter = 0u;
   unsigned char au8_blockFound = 0u;
   NVM_CheckType au8_NVM_WriteBlockState = 0u;

   /* search for the specified block specified in configurations array */
   for (; (unsigned char)NVM_NUM_OF_BLOCKS > au8_iter; au8_iter++)
   {
      if (NVM_BlocConfig[au8_iter].BlockId == BlockId)
      {
         unsigned char au8_dataIter = 0u;
         /* raise block found flag */
         au8_blockFound = 1;
         /* loop over the data with-in the block */
         for (; NVM_BlocConfig[au8_iter].BlockLength > au8_dataIter; au8_dataIter++)
         {
            /* copy/write the block into the mirror */
            *(NVM_BlocConfig[au8_iter].BlockRamAddress + au8_dataIter) = *(DataPtr + au8_dataIter);
         }
         /* notify upper layer */
         NVM_BlocConfig[au8_iter].BlockWriteNotifPtr();
         /* report success */
         au8_NVM_WriteBlockState = NVM_OK;
      }
   }
   if (!au8_blockFound)
   {
      /* report fail */
      au8_NVM_WriteBlockState = NVM_NOK;
   }
   /* report nvm block write state */
   return au8_NVM_WriteBlockState;
}

/*
*  Description : NVM runnable.
*
*  @param void 
*
*  @return void 
*/
void NVM_Main(void)
{   
   while (1)
   {
      /* check on the state */
      switch (gu8_NVM_SM_State)
      {
      case NVM_IDLE:
         /* waiting for action */
         break;      
      case NVM_CHUNK_WRITE:;
         /*if: (check configuration array of block length vs block written counter) (&&) (MIF_block is written) */
         if((NVM_NUM_OF_BLOCKS > gu8_writtenBlockCounter) && (NVM_WRITE_BLOCK_DONE == gu8_writeBlockDone))
         {
            /* pull flag down */
            gu8_writeBlockDone = NVM_WRITE_BLOCK_WAIT;
            /* increment blocks written counter */
            gu8_writtenBlockCounter++;
            /* write next block to memory interface */
            MEMIF_ReqWriteBlock(NVM_BlocConfig[gu8_writtenBlockCounter].BlockId, NVM_BlocConfig[gu8_writtenBlockCounter].BlockRamAddress);
         }
         else if(NVM_NUM_OF_BLOCKS <= gu8_writtenBlockCounter)
         {
            /* move to IDLE */
            gu8_NVM_SM_State = NVM_IDLE;
            /* reinitialize counter */
            gu8_writtenBlockCounter = 0;
         }
         else
         {
         }
         break;
      case NVM_CHUNK_READ:;
         /*if: (check configuration array of block length vs block read counter) (&&) (MIF_block is read) */
         if((NVM_NUM_OF_BLOCKS > gu8_readBlockCounter) && (NVM_READ_BLOCK_DONE == gu8_readBlockDone))
         {
            /* pull flag down */
            gu8_readBlockDone = NVM_READ_BLOCK_WAIT;
            /* increment blocks read counter */
            gu8_readBlockCounter++;
            /* read next block from memory interface */
            MEMIF_ReqReadBlock(NVM_BlocConfig[gu8_readBlockCounter].BlockId, NVM_BlocConfig[gu8_readBlockCounter].BlockRamAddress);                   
         }
         else if(NVM_NUM_OF_BLOCKS <= gu8_readBlockCounter)
         {
            /* move to IDLE */
            gu8_NVM_SM_State = NVM_IDLE;
            /* reinitialize counter */
            gu8_readBlockCounter = 0;
         }
         else
         {
         }
         break;
      }
   }
}
void NVM_WriteBlockDoneNotif(void)
{
   /* raise write block done notification */
   gu8_writeBlockDone = NVM_WRITE_BLOCK_DONE;
}
void NVM_ReadBlockDoneNotif(void)
{
   /* raise read block done notification */
   gu8_readBlockDone = NVM_READ_BLOCK_DONE;
}