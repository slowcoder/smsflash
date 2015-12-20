#include "stm32f4xx_pwr.h"

#include "types.h"

void configureTimerForRunTimeStats(void)
{

}

unsigned long getRunTimeCounterValue(void)
{
  return 0;
}

void vApplicationIdleHook( void )
{
//	stopModeEntrys++;
	//PWR_EnterSTOPMode(PWR_MainRegulator_ON,PWR_STOPEntry_WFE);
}

void PreSleepProcessing(uint32 *ulExpectedIdleTime)
{
/* place for user code */ 
}

void PostSleepProcessing(uint32 *ulExpectedIdleTime)
{
/* place for user code */
}
