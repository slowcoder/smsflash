#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "log.h"
#include "cli.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#define MAX_CMD_LEN 128

int clicmd_UNAME(int argc,char **argv);
int clicmd_PS(int argc,char **argv);
int clicmd_INFO(int argc,char **argv);
int clicmd_FREE(int argc,char **argv);

static char currCmd[MAX_CMD_LEN];

extern void CAOS_OutputDebugString(const char *pzStr);

static void displayLine(void) {
	CAOS_OutputDebugString("\r\033[J> ");
	CAOS_OutputDebugString(currCmd);
}

int tx_clicmd(int argc,char **argv);

static void executeCommand(void) {
	char *params[8];
	int argc,o,s;

	//LOG("\rShould execute: \"%s\"",currCmd);

	// Tokenize
	argc = 0;
	o = s = 0;
	while(currCmd[o] != 0) {
		if( currCmd[o] == ' ') {
			params[argc] = &currCmd[s];
			currCmd[o] = 0;
			argc++;
			s = o + 1;
		}
		o++;
	}
	params[argc] = &currCmd[s];
	argc++;

	CAOS_OutputDebugString("\r\n");

	if(      strcasecmp(params[0],"ps")    == 0 ) clicmd_PS(argc,params);
	else if( strcasecmp(params[0],"uname") == 0 ) clicmd_UNAME(argc,params);
	else if( strcasecmp(params[0],"info")  == 0 ) clicmd_INFO(argc,params);
	else if( strcasecmp(params[0],"free")  == 0 ) clicmd_FREE(argc,params);
	//else if( strcasecmp(params[0],"tx")    == 0 ) tx_clicmd(argc,params);
	else if( strcasecmp(params[0],"help")  == 0 ) {
		LOG("Available commands: ps, uname, info, free");
	} else {
		if( currCmd[0] ) // Is there a command at all?
			LOG("Unknown command \"%s\"",params[0]);
		else
			LOG("");
	}

	// Reset
	currCmd[0] = 0;
}

static void appendChar(int ch) {
	int i;

	// 10 = enter, 13 = return, 8 = bksp

	if( (ch == 10) || (ch == 13) ) {
		executeCommand();
		return;
	}
	if( ch == 8 ) {
		i = 0;
		while(currCmd[i] != 0) i++;

		if( i >= 1 ) {
			currCmd[i-1] = 0;
		}
		return;
	}

	i = 0;
	while(currCmd[i] != 0) i++;

	currCmd[i] = ch;
	currCmd[i+1] = 0;
}

static osThreadId cliTaskHandle;
static osMessageQId uartRxQueue;

static uint8_t rxbuf[1];
void upper_UART_ISR(int i) {
//  led_set(0,1,0);
	rxbuf[0] = i;
	xQueueSendToBackFromISR(uartRxQueue,rxbuf,NULL);
//  led_set(0,0,0);
}

void StartCLITask(void const * argument) {

  displayLine();
  for(;;) {

    if( xQueueReceive(uartRxQueue,rxbuf,10) == pdTRUE ) {
      cli_keyPress(rxbuf[0]);
    }
  }
}

int cli_init(void) {
	currCmd[0] = 0;

	osMessageQDef(myQueue01, 16, uint8_t);
	uartRxQueue = osMessageCreate(osMessageQ(myQueue01), NULL);

	//osThreadDef(cliTask, StartCLITask, osPriorityBelowNormal, 0, 256);
	osThreadDef(cliTask, StartCLITask, osPriorityNormal, 0, 256);
	cliTaskHandle = osThreadCreate(osThread(cliTask), NULL);

	return 0;
}

int cli_keyPress(const int ch) {

	appendChar(ch);

	displayLine();

	return 0;
}
