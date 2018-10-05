#ifndef SPI_LIBRARY
#define SPI_LIBRARY

#include <stm32f0xx.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_spi.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_misc.h>

#define spi_cs_low() GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define spi_cs_high() GPIO_SetBits(GPIOA,GPIO_Pin_4)

void init_spi_gpio(void);
void init_spi_soft(void);
uint8_t TM_SPI_Send(SPI_TypeDef* SPIx, uint8_t data);
void config_spi_int(void);

#endif 
