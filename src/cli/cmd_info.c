#include "types.h"

#include "log.h"

int clicmd_INFO(int argc,char **argv) {
	uint16 *pFS;
	uint32 *pUID;

	//LOG("DeviceID: 0x%08x",HAL_GetDEVID());
#if 1 // 405 addresses...
	pFS = (uint16*)0x1FFF7A22;
	LOG("Flash size: %uKB",pFS[0]);

	pUID = (uint32*)0x1FFF7A10;
	LOG("Device UID: %08lx%08lx%08lx",pUID[2],pUID[1],pUID[0]);
#endif
	return 0;
}
