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
   /* Assign main call-backs */
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
   /* Initialize a block counter */
   unsigned char au8_blockCounter = 0u;
   unsigned char au8_blockReadState = 0u;
   NVM_CheckType au8_NVM_ReadAllState = 0u;
   while ((unsigned char)NVM_NUM_OF_BLOCKS > au8_blockCounter)
   {
      /* 1 - Read a block : readFromMIF() */
      au8_blockReadState = MEMIF_ReqReadBlock(NVM_BlocConfig[au8_blockCounter].BlockId, NVM_BlocConfig[au8_blockCounter].BlockRamAddress);
      /* 2 - check state of block reading? TRUE: read the next block readFromMIF() and report busy; FALSE:report error */
      switch (au8_blockReadState)
      {
      case MEMIF_BUSY:
         /* report busy */
         au8_NVM_ReadAllState = NVM_BUSY;
         break;
      case MEMIF_OK:
         /* increment blocks counter */
         au8_blockCounter++;
         /* check if the last block to be transmitted has been reached or not */
         if ((unsigned char)(NVM_NUM_OF_BLOCKS - 1u) == au8_blockCounter)
         {
            /* if last block is transmitted successfully : report NVM_OK*/
            au8_NVM_ReadAllState = NVM_OK;
         }
         else
         {
            /* report busy : which indicates in this case that contigious blocks are still being read */
            au8_NVM_ReadAllState = NVM_BUSY;
         }
         break;
      case MEMIF_NOK:
         /* report error */
         au8_NVM_ReadAllState = NVM_NOK;
         break;
      }
      /* incase of NVM_NOK : we have to break out of the loop */
      if (NVM_NOK == au8_NVM_ReadAllState)
      {
         break;
      }
   }
   /* Report success(finished all the blocks succssfully) or fail */
   return au8_NVM_ReadAllState;
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
   /* Initialize a block counter */
   unsigned char au8_blockCounter = 0u;
   unsigned char au8_blockWriteState = 0u;
   NVM_CheckType au8_NVM_WriteAllState = 0u;
   while ((unsigned char)NVM_NUM_OF_BLOCKS > au8_blockCounter)
   {
      /* 1 - write a block : writeBlocMIF() */
      au8_blockWriteState = MEMIF_ReqWriteBlock(NVM_BlocConfig[au8_blockCounter].BlockId, NVM_BlocConfig[au8_blockCounter].BlockRamAddress);
      /* 2 - check state of block writting? TRUE: write the next block writeBlocMIF() and report busy; FALSE:report error */
      switch (au8_blockWriteState)
      {
      case MEMIF_BUSY:
         /* report busy */
         au8_NVM_WriteAllState = NVM_BUSY;
         break;
      case MEMIF_OK:
         /* increment blocks counter */
         au8_blockCounter++;
         /* check if the last block to be written has been reached or not */
         if ((unsigned char)(NVM_NUM_OF_BLOCKS - 1u) == au8_blockCounter)
         {
            /* if last block is transmitted successfully : report NVM_OK*/
            au8_NVM_WriteAllState = NVM_OK;
         }
         else
         {
            /* report busy : which indicates in this case that contigious blocks are still being read */
            au8_NVM_WriteAllState = NVM_BUSY;
         }
         break;
      case MEMIF_NOK:
         /* report error */
         au8_NVM_WriteAllState = NVM_NOK;
         break;
      }
      /* incase of NVM_NOK : we have to break out of the loop */
      if (NVM_NOK == au8_NVM_WriteAllState)
      {
         break;
      }
   }
   /* Report success(finished all the blocks succssfully) or fail */
   return au8_NVM_WriteAllState;
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
   /* Checks NVM state-machin global state */
   unsigned char NVM_SM_State = NVM_IDLE;
   while (1)
   {
      /* check on the state */
      switch (NVM_SM_State)
      {
      case NVM_IDLE:
         /* waiting for action */
         break;
      case NVM_ERROR:
         /* Error Handling procedure according to different types of error */
         /* Update NVM SM's state to IDLE */
         NVM_SM_State = NVM_IDLE;
         break;
      case NVM_BLOCK_WRITE:;
         /* Read notification of the nvm-block-wrtie (success or fail) */
         /* report/notify block write (success or fail) to the upper layer */
         /* update NVM state according nvm-block-wrtie*/
//          switch (/*nvm - block - write - state*/)
//          {
//             /* case success : move to idle waiting for the next action */
//             /* case fail : move to NVM-error state to procced to the suitable error handling procedure */
//          }
         break;
      case NVM_BLOCK_READ:;
         /* Read notification of the nvm-block-read (success or fail) */
         /* report/notify block read (success or fail) to the upper layer */
         /* update NVM state according nvm-block-read*/
         
//          switch (/*nvm - block - read - state*/)
//          {
//             /* case success : move to idle waiting for the next action */
//             /* case fail : move to NVM-error state to procced to the suitable error handling procedure */
//          }
         
         break;
      case NVM_CHUNK_WRITE:;
         /* Read notification of the nvm-chunk-wrtie (success or fail) */
         /* report/notify chunk write (success or fail) to the upper layer */
         /* update NVM state according nvm-chunk-wrtie*/
//          switch (/*nvm - chunk - write - state*/)
//          {
//             /* case busy : don't update NVM_SM's state */
//             /* case success : move to idle waiting for the next action */
//             /* case fail : move to NVM-error state to procced to the suitable error handling procedure */
//          }
         break;
      case NVM_CHUNK_READ:;
         /* Read notification of the nvm-chunk-read (success or fail) */
         /* report/notify chunk read (success or fail) to the upper layer */
         /* update NVM state according nvm-chunk-read*/
//          switch (/*nvm - chunk - read - state*/)
//          {
//             /* case busy : don't update NVM_SM's state */
//             /* case success : move to idle waiting for the next action */
//             /* case fail : move to NVM-error state to procced to the suitable error handling procedure */
//          }
         break;
      }
   }
}
void NVM_WriteBlockDoneNotif(void)
{}
void NVM_ReadBlockDoneNotif(void)
{}