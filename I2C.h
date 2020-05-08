/*
* I2C.h
*
* Created: 28/04/2020 23:16:43
*  Author: sony
*/


#ifndef I2C_H_
#define I2C_H_
typedef enum {I2C_OK, I2C_NOK}I2C_CheckType;
typedef void (*I2C_CbkPftype)(void);
typedef struct 
{
	unsigned long int CPUFreq;
	unsigned short int PreScaler;
	unsigned long int TargerRate;
	I2C_CbkPftype ActionDoneCbkPtr;
}I2C_ConfigType;
void I2C_Init(void);
I2C_CheckType I2C_ReqWrite(unsigned char SlaveAddress, const unsigned char* DataPtr, unsigned char DataLen);
I2C_CheckType I2C_ReqRead(unsigned char SlaveAddress, unsigned char* DataPtr, unsigned char DataLen);
void I2C_Stop(void);



#endif /* I2C_H_ */