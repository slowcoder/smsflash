#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "uart.h"
#include "leds.h"
#include "cli/cli.h"
#include "types.h"
#include "log.h"

osThreadId defaultTaskHandle;

extern uint32 stopModeEntrys;


void StartDefaultTask(void const * argument)
{
	int i = 0;

	uart_init();
	cli_init();
	leds_init();
	uart_puts("Hello, world\r\n");


	for(;;) {

		leds_set(i);

//		LOG("Hello, task. i=%i  SME=%u",i++,stopModeEntrys);
		osDelay(20);

		i++;
	}
}

void CAOS_OutputDebugString(const char *pzStr) {
	uart_puts(pzStr);
}

int main(void) {


	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	osKernelStart();

  	// Should never reach this..
	for(;;);

	return 0;
}