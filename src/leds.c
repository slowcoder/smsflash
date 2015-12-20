#include "stm32f4xx_gpio.h"

int leds_init(void) {
	GPIO_InitTypeDef GPIO_InitDef;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;

	//Initialize pins
	GPIO_Init(GPIOA, &GPIO_InitDef);

	GPIO_SetBits(GPIOA,GPIO_Pin_8);

#if 1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;

	//Initialize pins
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitDef);

	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	GPIO_SetBits(GPIOC,GPIO_Pin_9);
#endif

	return 0;
}

int leds_set(uint8_t ledMask) {
	if( ledMask & (1<<0) ) GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	else                   GPIO_SetBits(GPIOA,GPIO_Pin_8);

	if( ledMask & (1<<1) ) GPIO_ResetBits(GPIOC,GPIO_Pin_9);
	else                   GPIO_SetBits(GPIOC,GPIO_Pin_9);

	if( ledMask & (1<<2) ) GPIO_ResetBits(GPIOC,GPIO_Pin_8);
	else                   GPIO_SetBits(GPIOC,GPIO_Pin_8);

	if( ledMask & (1<<3) ) GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	else                   GPIO_SetBits(GPIOC,GPIO_Pin_7);

	if( ledMask & (1<<4) ) GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	else                   GPIO_SetBits(GPIOC,GPIO_Pin_6);

	return 0;
}
