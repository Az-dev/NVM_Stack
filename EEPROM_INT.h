/*
* EEPROM_INT.h
*
* Created: 28/04/2020 23:25:12
*  Author: sony
*/


#ifndef EEPROM_INT_H_
#define EEPROM_INT_H_
typedef enum {EEINT_OK, EEINT_NOK, EEINT_BUSY}EEINT_CheckType;
typedef void (*EEINT_CbkFunPtrType)(void);
typedef struct
{
	EEINT_CbkFunPtrType WriteDoneCbkPtr;
	EEINT_CbkFunPtrType ReadDoneCbkPtr;
}EEINT_ConfigType;
void EEINT_Init(void);
EEINT_CheckType EEINT_ReqWrite(unsigned char StartAddress, const unsigned char* DataPtr, unsigned char Length);
EEINT_CheckType EEINT_ReqRead(unsigned char StartAddress,unsigned char* DataPtr,unsigned char Length);
void EEINT_Main(void);


#endif /* EEPROM_INT_H_ */