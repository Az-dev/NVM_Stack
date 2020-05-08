/*
 * EEPROM_EXT.h
 *
 * Created: 28/04/2020 23:39:42
 *  Author: sony
 */ 


#ifndef EEPROM_EXT_H_
#define EEPROM_EXT_H_
typedef enum {EEEXT_OK, EEEXT_NOK, EEEXT_BUSY}EEEXT_CheckType;
	typedef void (*EEEXT_CbkFunPtrType)(void);
	typedef struct
	{
		EEEXT_CbkFunPtrType WriteDoneCbkPtr;
		EEEXT_CbkFunPtrType ReadDoneCbkPtr;
	}EEEXT_ConfigType;
void EEEXT_Init(void);
EEEXT_CheckType EEEXT_ReqWrite(unsigned char StartAddress, const unsigned char* DataPtr, unsigned char Length);
EEEXT_CheckType EEEXT_ReqRead(unsigned char StartAddress,unsigned char* DataPtr,unsigned char Length);
void EEEXT_Main(void);
void EEXT_ACtionDoneCallback(void);




#endif /* EEPROM_EXT_H_ */