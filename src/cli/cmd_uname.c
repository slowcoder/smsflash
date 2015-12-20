#include "types.h"

#include "FreeRTOS.h"
#include "task.h"

#include "log.h"

#ifndef GIT_BASE_REVISION
#define GIT_BASE_REVISION "NotInGIT"
#endif

int clicmd_UNAME(int argc,char **argv) {
	uint32 v;

//	v = HAL_GetHalVersion();
	v = 0;

	LOG("Rev %s - FreeRTOS %s - STM32 HAL v%u.%u.%u - Built %s %s",
		GIT_BASE_REVISION,
		tskKERNEL_VERSION_NUMBER,
		(v >> 24) & 0xFF,
		(v >> 16) & 0xFF,
		(v >>  8) & 0xFF,
		__DATE__,
		__TIME__);

	return 0;
}
