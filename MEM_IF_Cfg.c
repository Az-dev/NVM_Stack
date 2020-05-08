/*
* MEM_IF_Cfg.c
*
* Created: 29/04/2020 00:19:22
*  Author: sony
*/
#include "MEM_IF.h"
#include "MEM_IF_Cfg.h"
#include "NVM.h"
const MEMIF_BlockConfigType MEMIF_BlockConfig[MEMIF_NUM_OF_BLOCKS] =
{
	{
		0,
		1,
		INTERNAL_MEM,
		0x00,
	},
	{
		1,
		1,
		EXTERNAL_MEM,
		0x00,
	}
};
const MEMIF_ConfigType MEMIF_ConfigParam =
{
	MEMIF_BlockConfig,
	NVM_WriteBlockDoneNotif,
	NVM_ReadBlockDoneNotif
};