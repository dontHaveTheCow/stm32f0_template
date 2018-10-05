#ifndef SERIAL_LIBRARY
#define SERIAL_LIBRARY

//These are the Includes
#include <stm32f0xx_gpio.h>
#include <stm32f0xx_usart.h>
#include <stm32f0xx_misc.h>
#include <stm32f0xx_rcc.h>

#define BAUD_9600 9600
#define BAUD_4800 4800

#define USART_PORT GPIOA
#define USART_TX GPIO_Pin_9
#define USART_RX GPIO_Pin_10

//These are the prototypes for the routines
void Serial_init(int baudrate);
void Serial_send(uint8_t data);
void Serial_sendStr(char* string);
uint16_t Serial_rec(void);
void Serial_config_int(void);

#endif