/*
 * EEPROM_INT.c
 *
 * Created: 28/04/2020 23:37:28
 *  Author: sony
 */

#include "EEPROM_INT.h"
#include "EEPROM_INT_CFG.h"
#include "avr/io.h"

#define IDLE 1
#define BUSY 2
#define DONE_WR 3
#define DONE_RD 4

static uint8_t EEINT_SM1;
static uint8_t EEINT_SM2;

void EEINT_Init(void)
{
   EEINT_SM1 = IDLE;
   EEINT_SM2 = IDLE;
}

void EEPROM_WriteByte(uint16_t au16_EEPROM_Address, uint8_t au8_EEPROM_Data)
{
   while (EECR & (1 << EEWE))
      ; // Wait Until The Last Write Operation IS Complete

   EEAR = au16_EEPROM_Address;
   EEDR = au8_EEPROM_Data;

   EECR |= (1 << EEMWE); // EEPROM Master Write Enable
   EECR |= (1 << EEWE);  // Start eeprom write by setting EEWE
}

uint8_t EEPROM_ReadByte(uint16_t au16_EEPROM_Address)
{
   while (EECR & (1 << EEWE))
      ; // Wait Until The Last Operation IS Complete

   EEAR = au16_EEPROM_Address;
   EECR |= (1 << EERE); // start EEPROM read by setting EERE

   return EEDR; // Return data from the memory location
}

static void EEPROM_WriteBytes(uint16_t au16_EEPROM_Address, uint8_t *u8ptr_Data, uint16_t u16_DataLength)
{
   while (u16_DataLength != 0)
   {
      EEPROM_WriteByte(au16_EEPROM_Address, *u8ptr_Data);
      au16_EEPROM_Address++;
      u8ptr_Data++;
      u16_DataLength--;
   }
}
static void EEPROM_ReadBytes(uint16_t au16_EEPROM_Address, uint8_t *u8ptr_Data, uint16_t u16_DataLength)
{
   while (u16_DataLength != 0)
   {
      *u8ptr_Data = EEPROM_ReadByte(au16_EEPROM_Address); //Read a byte from EEPROM to RAM
      au16_EEPROM_Address++;                              //Increment the EEPROM Address
      u8ptr_Data++;                                       //Increment the RAM Address
      u16_DataLength--;                                   //Decrement NoOfBytes after Reading each Byte
   }
}

EEINT_CheckType EEINT_ReqWrite(unsigned char StartAddress, const unsigned char *DataPtr, unsigned char Length)
{
   EEINT_CheckType EEINT_Status = EEINT_BUSY;
   EEINT_SM1 = BUSY;

   if (DataPtr != ((void *)0))
   {
      if (Length < EEINT_MAX_SIZE)
      {
         // Continuously Write The Data Bytes
         EEPROM_WriteBytes(StartAddress, (unsigned char *)DataPtr, Length);
         EEINT_Status = EEINT_OK;
         EEINT_SM1 = DONE_WR;
      }
      else
      {
         // Memory Size Limit Exceeded
         EEINT_Status = EEINT_NOK;
      }
   }
   else
   {
      // Non-Valid Data Pointer Is Passed
      EEINT_Status = EEINT_NOK;
   }

   return EEINT_Status;
}

EEINT_CheckType EEINT_ReqRead(unsigned char StartAddress, unsigned char *DataPtr, unsigned char Length)
{
   EEINT_CheckType EEINT_Status = EEINT_BUSY;
   EEINT_SM2 = BUSY;

   if (DataPtr != ((void *)0))
   {
      if (Length < EEINT_MAX_SIZE)
      {
         // Continuously Read The Data Bytes
         EEPROM_ReadBytes(StartAddress, DataPtr, Length);
         EEINT_Status = EEINT_OK;
         EEINT_SM2 = DONE_RD;
      }
      else
      {
         // Memory Size Limit Exceeded
         EEINT_Status = EEINT_NOK;
      }
   }
   else
   {
      // Non-Valid Data Pointer Is Passed
      EEINT_Status = EEINT_NOK;
   }

   return EEINT_Status;
}

void EEINT_Main(void)
{
   // Done Writing State
   if (EEINT_SM1 == DONE_WR)
   {
      EEINT_SM1 = IDLE;
      EEINT_ConfigParam.WriteDoneCbkPtr();
   }
   else if (EEINT_SM1 == EEINT_NOK)
   {
      EEINT_ConfigParam.WriteDoneCbkPtr();
   }
   // Done Reading State
   if (EEINT_SM2 == DONE_RD)
   {
      EEINT_SM1 = IDLE;
      EEINT_ConfigParam.ReadDoneCbkPtr();
   }
   else if (EEINT_SM2 == EEINT_NOK)
   {
      EEINT_ConfigParam.ReadDoneCbkPtr();
   }
}