#include "spi.h"

void init_spi_gpio(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    // PA5 - SCK PA6 - MISO PA7 - MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //PA4 - CS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void init_spi_soft()
{
    SPI_InitTypeDef SPI_InitStruct;

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_0);

    //Set CS high (slave non selected)
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; //Clock is low when idle
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; //Data sampled at 1st edge
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; //Was Hard
    SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStruct);

    SPI_SSOutputCmd(SPI1,ENABLE);
    SPI_Cmd(SPI1, ENABLE);
}

uint8_t TM_SPI_Send(SPI_TypeDef* SPIx,  uint8_t data){
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_SendData8(SPI1,data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
    //while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_ReceiveData8(SPI1);

}

void config_spi_int(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x0F;
    NVIC_Init(&NVIC_InitStructure);
}