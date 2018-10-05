#include "serial.h"

void Serial_init(int baudrate){

		/* Enable GPIO clock */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

		/* Enable USART clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;
	    USART_InitTypeDef USART_InitStructure;

	    /* Configure USART Tx, Rx as alternate function push-pull */
	    GPIO_InitStructure.GPIO_Pin = USART_TX | USART_RX;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);

	    USART_InitStructure.USART_BaudRate = baudrate;
	    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	    USART_InitStructure.USART_StopBits = USART_StopBits_1;
	    USART_InitStructure.USART_Parity = USART_Parity_No;
	    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	    /* Connect PXx to USARTx_Tx */
	    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);

	    /* Connect PXx to USARTx_Rx */
	    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

	    /* USART configuration */
	    USART_Init(USART1, &USART_InitStructure);

	    /* Enable USART */
	    USART_Cmd(USART1, ENABLE);



}

void Serial_send(uint8_t data)
{
	USART_SendData(USART1,data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC ) == RESET)
	{
	}
}

void Serial_sendStr(char* string){
	while(*string != 0){
		Serial_send(*string++);
	}
}

uint16_t Serial_rec(void)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USART1);
}

void Serial_config_int(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;

	  /* Enable the USART1 Interrupt */
	  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

