/*
 * I2C_Cfg.c
 *
 * Created: 28/04/2020 23:21:24
 *  Author: sony
 */ 
#include "I2C.h"
#include "EEPROM_EXT.h"
const I2C_ConfigType I2C_ConfigParam = 
{
	8000000ul,
	1,
	100000,
	EEXT_ACtionDoneCallback
};