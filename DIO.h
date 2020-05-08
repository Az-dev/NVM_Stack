#ifndef DIO_H
#define DIO_H
typedef struct 
{
	unsigned char Mask;
	unsigned char Port;
	unsigned char Direction;
	unsigned char UsePullUp;
}DioConfigParamType;
void DIO_Init(void);
void DIO_Write(unsigned char GroupId, unsigned char Data);
void DIO_Read(unsigned char GroupId, unsigned char * DataPtr);


#endif