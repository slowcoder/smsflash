#include "stm32f4xx.h"
#include "stm32f4xx_pwr.h"

#define FREQ_180 1
#define FREQ_168 0

#define VECT_TAB_OFFSET  0x00

uint32_t SystemCoreClock = 16000000;

void SystemClockUp(void);

/*
 * Resets RCC and disables interrupts.
 * I.e, we're running at 16MHz, clocked directly by the HSI with an
 * AHB prescaler of 1
 */
void SystemInit(void) {
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x24003010;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIR = 0x00000000;

  /* Configure the Vector Table location add offset address ------------------*/
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */

  SystemClockUp();
}

#if FREQ_168 // 168MHz
#define PLL_M      16
#define PLL_N      336
#define PLL_P      2
#define PLL_Q      7
#endif

#if FREQ_180 // 180MHz
#define PLL_M      10
#define PLL_N      225
#define PLL_P      2
#define PLL_Q      7
#endif

void SystemClockUp(void) {
  // Regulator power-scale 1
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  PWR->CR |= PWR_CR_VOS;

  // Core overdrive
#if FREQ_180
  PWR_OverDriveCmd(ENABLE);
#endif

  /* HCLK = SYSCLK / 1*/
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
      
  /* PCLK2 = HCLK / 2*/
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
  /* PCLK1 = HCLK / 1*/
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

  /* Configure the main PLL */
  RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                 (RCC_PLLCFGR_PLLSRC_HSI) | (PLL_Q << 24);

  /* Enable the main PLL */
  RCC->CR |= RCC_CR_PLLON;

  /* Wait till the main PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) == 0)
  {
  }
   
  /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
  FLASH->ACR = FLASH_ACR_PRFTEN |FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_7WS;

  /* Select the main PLL as system clock source */
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  /* Wait till the main PLL is used as system clock source */
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
  {
  }


#if FREQ_168
  SystemCoreClock = 168000000;
#endif
#if FREQ_180
  SystemCoreClock = 168000000;
#endif
}