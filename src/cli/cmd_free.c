#include "FreeRTOS.h"
#include "portable.h"

#include "log.h"

int clicmd_FREE(int argc,char **argv) {
	size_t freeram;

	freeram = xPortGetFreeHeapSize();
	LOG("Heap free: %lu",freeram);

	return 0;
}
