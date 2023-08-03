/*
 * usart.c - serial i/o routines for STM32F030F4
 * 05-30-15 E. Brombaugh
 */
 
#include <stdio.h>
#include "usart.h"

#define USE_IRQ
#ifdef USE_IRQ
uint8_t RX_buffer[256];
uint8_t *RX_wptr, *RX_rptr;
#endif

/* USART1 setup */
void usart_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Setup USART */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Connect PA9 to USART1_Tx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);

	/* Connect PA10 to USART1_Rx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function w/ pullup */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART configuration */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* USART = 115k-8-N-1 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
#ifdef USE_IRQ
	/* init RX buffer write/read pointers*/
	RX_wptr = &RX_buffer[0];
	RX_rptr = &RX_buffer[0];

	/* Enable RX interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* Enable the USART6 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}

/*
 * output for tiny printf
 */
void usart_putc(void* p, char c)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{}
	USART_SendData(USART1, (uint8_t) c);
}

/*
 * input 
 */
int16_t usart_getc(void)
{
#ifndef USE_IRQ
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		return EOF;
	else
		return USART_ReceiveData(USART1);
#else
	/* interrupt version */
	int retval;
	
	/* check if there's data in the buffer */
	if(RX_rptr != RX_wptr)
	{
		/* get the data */
		retval = *RX_rptr++;
		
		/* wrap the pointer */
		if((RX_rptr - &RX_buffer[0])>=256)
			RX_rptr = &RX_buffer[0];
	}
	else
		retval = EOF;

	return retval;
#endif
}

#ifdef USE_IRQ
/*
 * USART IRQ handler - used only for Rx for now
 */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/* get the character */
		uint8_t rxchar = USART_ReceiveData(USART1);

		/* check if there's room in the buffer */
		if((RX_wptr != RX_rptr-1) &&
           (RX_wptr - RX_rptr != 255))
		{
			/* Yes - Queue the new char */
			*RX_wptr++ = rxchar;
	
			/* Wrap pointer */
			if((RX_wptr - &RX_buffer[0])>=256)
				RX_wptr = &RX_buffer[0];
		}
	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
	}
}
#endif