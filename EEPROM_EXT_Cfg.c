/*
 * EEPROM_EXT_Cfg.c
 *
 * Created: 28/04/2020 23:52:21
 *  Author: sony
 */ 
#include "EEPROM_EXT.h"
#include "MEM_IF.h"
const EEEXT_ConfigType EEXT_ConfigParam =
{
	MEMIF_ExtEepromWriteCbk,
	MEMIF_ExtEepromReadCbk
};