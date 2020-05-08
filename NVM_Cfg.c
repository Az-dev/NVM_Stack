/*
* NVM_Cfg.c
*
* Created: 29/04/2020 20:17:39
*  Author: sony
*/
#include "NVM.h"
#include "NVM_Cfg.h"
#include "NVM_USER.h"
#include "App1.h"
#include "App2.h"
void Main_WriteAllDone(void);
void Main_ReadAllDone(void);
const NVM_ConfigType NVM_BlocConfig[NVM_NUM_OF_BLOCKS] =
{
	{
		0,
		1,
		Counter1RamMirrorBlock,
		App1_WriteCbk,
		App1_ReadCbk
	},
	{
		1,
		1,
		Counter2RamMirrorBlock,
		App2_WriteCbk,
		App2_ReadCbk
	}
};
const NVM_GlobalCallbacksType NVM_GlobalCallbacks = 
{
	Main_WriteAllDone,
	Main_ReadAllDone
};