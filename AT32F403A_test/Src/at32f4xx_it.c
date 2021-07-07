/**
  ******************************************************************************
  * File   : USART/Printf/at32f4xx_it.c
  * Version: V1.3.0
  * Date   : 2021-03-18
  * Brief  : Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx_it.h"
#include "at32_board.h"
#include "at32f4xx_dma.h"
#include "stdio.h"
/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
/* void SVC_Handler(void) //注释掉
{
  static uint32_t i = 0;
  i++;
  if (i == 10)
  {
    AT32_LEDn_Toggle(LED2);
    i = 0;
  }
} */

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
/* void PendSV_Handler(void) //注释掉
{
  static uint32_t i = 0;
  i++;
  if (i == 10)
  {
    AT32_LEDn_Toggle(LED4);
    i = 0;
  }
}
 */
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/* void SysTick_Handler(void) //注释掉
{
  static uint32_t i = 0;
  i++;
  if (i == 10)
  {
    AT32_LEDn_Toggle(LED3);
    i = 0;
  }
} */

/**
  * @}
  */
extern uint8_t RxBuffer1[100];
uint8_t USART1_END = 1,
        USART_INT_RDNE_NUM = 0,
        USART_INT_IDLEF_NUM = 0,
        intrupt_NUM = 0,
        USART_INT_TRAC_NUM = 0;

void USART1_IRQHandler(void)
{
  //AT32_LEDn_ON(LED2);
  intrupt_NUM++;

  if (USART_GetITStatus(USART1, USART_INT_RDNE) != RESET)
  {
    /* Read one byte from the receive data register */
    //RxBuffer1[1] = USART_ReceiveData(USART1);//需要读取数据清除空闲标志位，否则会循环中断
    USART_ClearFlag(USART1, USART_FLAG_RDNE);
    //  AT32_LEDn_Toggle(LED2);
    RxBuffer1[USART_INT_RDNE_NUM] = USART_ReceiveData(USART1);
    USART_INT_RDNE_NUM++;
    if (USART_INT_RDNE_NUM > 98)
      USART_INT_RDNE_NUM = 98;
  }
  if (USART_GetITStatus(USART1, USART_INT_IDLEF) != RESET)
  {
    /* Read one byte from the receive data register */
    //    RxBuffer1[2] = USART1 -> STS;
    RxBuffer1[USART_INT_RDNE_NUM - 1] = USART_ReceiveData(USART1); //需要读取数据用于清除空闲标志位，否则会死机
    USART_INT_RDNE_NUM = 0;

    USART_INT_IDLEF_NUM++;
    USART1_END = 0;
  }
  //printf(" %d %d %d", intrupt_NUM, USART_INT_RDNE_NUM, USART_INT_IDLEF_NUM);
  if (USART_GetITStatus(USART1, USART_INT_TRAC) != RESET)
  {
    USART_INT_TRAC_NUM++;
  }
}

/**
  * @brief  This function handles DMA1 Channel4.
  * @param  None
  * @retval None
  */
* /
    void DMA1_Channel4_IRQHandler(void)
{
  if (DMA_GetITStatus(DMA1_INT_GL4) == SET)
  {
    DMA_ClearFlag(DMA1_FLAG_GL4);
  }
  if (DMA_GetITStatus(DMA1_INT_HT4) == SET)
  {
    DMA_ClearFlag(DMA1_FLAG_TC4);
  }
  if (DMA_GetITStatus(DMA1_INT_TC4) == SET)
  {
    DMA_ClearFlag(DMA1_FLAG_HT4);
    ;
  }
  if (DMA_GetITStatus(DMA1_INT_ERR4) == SET)
  {
    DMA_ClearFlag(DMA1_FLAG_ERR4);
    ;
  }
}

/**
  * @brief  This function handles DMA1 Channel4.
  * @param  None
  * @retval None
  */
void DMA1_Channel5_IRQHandler(void)
{
  if (DMA_GetITStatus(DMA1_INT_GL5) == SET)
  {
    DMA_ClearFlag(DMA1_FLAG_GL5);
    ;
  }
  if (DMA_GetITStatus(DMA1_INT_HT5) == SET)
  {
    DMA_ClearFlag(DMA1_FLAG_TC5);
    ;
  }
  if (DMA_GetITStatus(DMA1_INT_TC5) == SET)
  {
    DMA_ClearFlag(DMA1_FLAG_HT5);
    ;
  }
  if (DMA_GetITStatus(DMA1_INT_ERR5) == SET)
  {
    DMA_ClearFlag(DMA1_FLAG_ERR5);
    ;
  }
}git
