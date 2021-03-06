/**
  ******************************************************************************
  * File   : USART/Printf/main.c 
  * Version: V1.3.0
  * Date   : 2021-03-18
  * Brief  : Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at32_board.h"
#include "at32f4xx.h"
#include "usart.h"
#include <stdio.h>

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
extern void test(void);
int main(void)
{
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */

  AT32_Board_Init();
  // UART_Print_Init(115200);
  MX_USART1_UART_Init();

  /* Output a message on Hyperterminal using printf function */
  printf("USART Printf Example: retarget the C library printf function to the USART\n\r");
  printf("/****0x%p****/\n", USART1);
  printf("/****0x%p****/\n", &(USART1->DT));
  test();

  while (1)
  {
    ;
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */
