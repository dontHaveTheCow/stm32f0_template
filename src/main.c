#include <stm32f0xx.h>
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>
#include "delay.h"
#include "serial.h"
#include "spi.h"
#include "stdio.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/* STM32f0 discovery board func*/
void InitializeDiscoveryLEDs(uint16_t led_pins)
{
    __asm("nop");

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
}

extern void initialise_monitor_handles(void);

int main(void)
{
    //Tag id : 0x42BE24D91
    printf("Program started!!!\r\n");
    /*Stm32 specific initializations*/
    init__delay();
    InitializeDiscoveryLEDs(GPIO_Pin_8 | GPIO_Pin_9);

    while(1){
        
        _delay_ms(300);
        GPIO_SetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9);
        _delay_ms(300);
        GPIO_ResetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9);
    }
}
