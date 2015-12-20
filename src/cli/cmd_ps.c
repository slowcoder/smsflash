#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "log.h"

static const char *taskStates[] = { "Unknown","Ready","Running","Blocked","Suspended","Deleted" };
int clicmd_PS(int argc,char **argv) {
	TaskStatus_t *pTS;
	unsigned long uTotalRuntime;
	UBaseType_t uNumTasks;
	int i;

	uNumTasks = uxTaskGetNumberOfTasks();
	pTS = malloc(sizeof(TaskStatus_t) * uNumTasks);
	if( pTS == NULL ) {
		LOGE("Out of memory..");
	}

	uNumTasks = uxTaskGetSystemState(pTS,uNumTasks,&uTotalRuntime);
	LOG("Found %u tasks - Total runtime: %lu",uNumTasks,uTotalRuntime);

	for(i=0;i<uNumTasks;i++) {
		const char *pzState;
		int32_t pct;

		pzState = taskStates[0];
		switch(pTS[i].eCurrentState) {
			case eRunning:   pzState = taskStates[2]; break;
			case eReady:     pzState = taskStates[1]; break;
			case eBlocked:   pzState = taskStates[3]; break;
			case eSuspended: pzState = taskStates[4]; break;
			case eDeleted:   pzState = taskStates[5]; break;
		}

		pct = pTS[i].ulRunTimeCounter;
		pct *= 100;
		pct = pct / uTotalRuntime;

		LOG("%16s: %9s %3lu%% SHW:%u",
			pTS[i].pcTaskName,
			pzState,
			pct,
//			(pTS[i].ulRunTimeCounter*1),
			pTS[i].usStackHighWaterMark);
	}

	free(pTS);

	return 0;
}
