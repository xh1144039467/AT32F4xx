
/**
  * @brief  initialize UART1   
  * @param  bound: UART BaudRate
  * @retval None
  */
#include "usart.h"
#include "at32_board.h"
#include "at32f4xx_dma.h"

int8_t TxBuffer1[100] = {0}, TxBufferSize1 = sizeof(TxBuffer1);
int8_t RxBuffer1[100] = {0}, RxBufferSize1 = sizeof(RxBuffer1);
USART_InitType USART1_InitStructure, USART2_InitStructure;

/* USART1 init function */
void MX_USART1_UART_Init(void)
{
    //时钟要先开启初始化，要不然会出问题
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_USART1, ENABLE);
    USART1_InitStructure.USART_BaudRate = 115200;
    USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART1_InitStructure.USART_StopBits = USART_StopBits_1;
    USART1_InitStructure.USART_Parity = USART_Parity_No;
    USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART1_InitStructure);
    UART1_MspInit();
    USART_Cmd(USART1, ENABLE);
}

void UART1_MspInit(void)
{
    GPIO_InitType GPIO_InitStructure;
/* USART1 clock enable */
#if defined(AT32F421xx)
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_GPIOA, ENABLE);    //使能串口引脚时钟
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE); //使能串口时钟
#endif
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE);
    /* Configure the UART1 TX pin */
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_9;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
#if !defined(AT32F421xx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
#else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
#endif
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure the UART1 RX pin */
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_10; //PA10
#if !defined(AT32F421xx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
#else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PU;
#endif
    GPIO_Init(GPIOA, &GPIO_InitStructure);

#if defined(AT32F421xx)
    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource10, GPIO_AF_1);
#endif
    USART1_NVIC_Configuration();
    /*     USART_INTConfig(USART1, USART_INT_RDNE, ENABLE);*/
    //USART_INTConfig(USART1, USART_INT_TRAC, ENABLE);
    USART_INTConfig(USART1, USART_INT_RDNE, ENABLE);
    //USART_INTConfig(USART1, USART_INT_TRAC, ENABLE);
    USART_INTConfig(USART1, USART_INT_IDLEF, ENABLE);
    /*     USART1->STS = 0x80;
    printf("-------0x%X-----",USART1->STS ); */

    /* Enable the USART1 Interrupt */

    /* USART1 DMA Init */
    //    DMA_Configuration();
}

void DMA_Configuration(void)
{
    DMA_InitType DMA_InitStructure;

    /* USART1_Tx_DMA_Channel (triggered by USART1 Tx event) Config */
    DMA_Reset(DMA1_Channel4);
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DT));
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer1;
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;
    DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

    /* USART1_Tx_DMA_Channel (triggered by USART1 Rx event) Config */
    DMA_Reset(DMA1_Channel5);
    DMA_DefaultInitParaConfig(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DT));
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer1;
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALSRC;
    DMA_InitStructure.DMA_BufferSize = RxBufferSize1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE;
    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_VERYHIGH;
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    /* Enable USART2 DMA TX Channel */
    DMA_ChannelEnable(DMA1_Channel4, ENABLE);
    DMA_ChannelEnable(DMA1_Channel5, ENABLE);
    /* Enables  Channelx */
    DMA_INTConfig(DMA1_Channel4, DMA_INT_TC, ENABLE);
    DMA_INTConfig(DMA1_Channel5, DMA_INT_TC, ENABLE);
    /* Enable USART2 DMA TX request */
    USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
}

void USART1_NVIC_Configuration(void)
{
    NVIC_InitType NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void vUART1_MspDeInit()
{
}

/**********************************************************************串口二的配置****************************************************************************/
/* USART1 init function */
void MX_USART2_UART_Init(void)
{
    //时钟要先开启初始化，要不然会出问题
    RCC_APB1PeriphClockCmd(RCC_APB2PERIPH_GPIOA | RCC_APB1PERIPH_USART2, ENABLE);
    USART2_InitStructure.USART_BaudRate = 115200;
    USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART2_InitStructure.USART_StopBits = USART_StopBits_1;
    USART2_InitStructure.USART_Parity = USART_Parity_No;
    USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART2_InitStructure);
    UART2_MspInit();
    USART_Cmd(USART2, ENABLE);
}

void UART2_MspInit(void)
{
    GPIO_InitType GPIO_InitStructure;
/* USART1 clock enable */
#if defined(AT32F421xx)
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_GPIOA, ENABLE);    //使能串口引脚时钟
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE); //使能串口时钟
#endif

    AT32_PRINT_UART_RCC_CLK_FUNC;
    /* Configure the UART1 TX pin */
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2; //PA2
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
#if !defined(AT32F421xx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
#else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;
#endif
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure the UART1 RX pin */
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_3; //PA3
#if !defined(AT32F421xx)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
#else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pull = GPIO_Pull_PU;
#endif
    GPIO_Init(GPIOA, &GPIO_InitStructure);

#if defined(AT32F421xx)
    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinsSource10, GPIO_AF_1);
#endif
    USART_INTConfig(USART2, USART_INT_RDNE, ENABLE);
    USART_INTConfig(USART2, USART_INT_TRAC, ENABLE);
}
void USART2_NVIC_Configuration(void)
{
    NVIC_InitType USART2_NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USART1 Interrupt */
    USART2_NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    USART2_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    USART2_NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    USART2_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&USART2_NVIC_InitStructure);
}
