#ifndef __usart_H
#define __usart_H
#include <at32f4xx.h>

void MX_USART1_UART_Init(void);
void UART1_MspInit(void);

void USART1_NVIC_Configuration(void);



void MX_USART2_UART_Init(void);
void UART2_MspInit(void);

void USART2_NVIC_Configuration(void);


void DMA_Configuration(void);
void USART1_NVIC_Configuration(void);

#endif /*__ usart_H */
