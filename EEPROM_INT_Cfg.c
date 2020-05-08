/*
 * EEPROM_INT_Cfg.c
 *
 * Created: 28/04/2020 23:45:01
 *  Author: sony
 */ 
#include "EEPROM_INT.h"
#include "MEM_IF.h"
const EEINT_ConfigType EEINT_ConfigParam = 
{
	MEMIF_IntEepromWriteCbk,
	MEMIF_IntEepromReadCbk
};