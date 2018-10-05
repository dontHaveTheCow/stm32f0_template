#ifndef DELAY_LIBRARY
#define DELAY_LIBRARY

//These are the Includes
#include <stm32f0xx.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>

//These are the Define statements
static __IO uint32_t sysTickCounter;
//These are the prototypes for the routines
void init__delay(void);
void _delay_ms(int ms);

#endif

