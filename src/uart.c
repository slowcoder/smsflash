#include <string.h>

#include "stm32f4xx_usart.h"
#include "misc.h"

#include "types.h"
#include "uart.h"
#include "cli/cli.h"

#define RX_INTERRUPT

void upper_UART_ISR(int i);
uint32 stopModeEntrys = 0;

#define USART_FLAG_ERRORS (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE)


void USART1_IRQHandler(void) {
	int i;

	stopModeEntrys++;

//	return;

	if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)           
    {          
		i = USART_ReceiveData(USART1);

		USART_ClearFlag(USART2,USART_FLAG_RXNE);
	//     USART_GetFlagStatus(USART2,USART_FLAG_ORE);


		upper_UART_ISR(i);
	}

	uint32 status;
	status = USART1->SR;
    while(status & (USART_FLAG_RXNE |    USART_FLAG_ERRORS))
    {
        uint8 ch;

        ch = (volatile uint32_t)USART1->DR;    // Read Data, or Clear Error(s)
        (void)ch;

        status = USART1->SR;
    }

}


int uart_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// PA10 = RX
	// PA9  = TX

	// Clock the GPIO bank
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	// Clock the UART peripheral
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

#ifdef RX_INTERRUPT
	NVIC_InitTypeDef NVIC_InitStructure;

	memset(&NVIC_InitStructure,0,sizeof(NVIC_InitTypeDef));

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_SetPriority(USART1_IRQn,5);
#endif

	// Set up AFs
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	// Configure the GPIOs to be routed as AFs
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStruct);

	USART_ClearFlag(USART1, USART_IT_RXNE);
    USART_ClearFlag(USART1, USART_IT_TXE);
#if 0
	uint32 status;
	status = USART1->SR;
#define USART_FLAG_ERRORS (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE)
    while(status & (USART_FLAG_RXNE |    USART_FLAG_ERRORS))
    {
        uint8 ch;

        ch = USART1->DR;    // Read Data, or Clear Error(s)

        if (status & USART_FLAG_ERRORS) {
        }
        else {
//            Fifo_Insert(Uart->Rx, sizeof(ch), &ch);
        }

        status = USART1->SR;
    }
#endif

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);

#ifdef RX_INTERRUPT
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif

	return 0;
}

int uart_puts(const char *pzStr) {
	char *pS;

	pS = (char *)pzStr;
	while( *pS != 0 ) {
		USART_SendData(USART1, (uint8_t)*pS);

		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}

		pS++;
	}

	return 0;
}
