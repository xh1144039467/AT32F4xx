/* #ifndef __TEST__
#define __TEST__


#endif */
#include "FreeRTOS.h"
#include "at32_board.h"
#include "at32f4xx.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "task.h"
TaskHandle_t StartTask_Handler;
TaskHandle_t ButtonTask_Handler;
QueueHandle_t Button_Queue;

void start_task(void);
void Button_task(void);

void test(void)
{
    taskENTER_CRITICAL();

    Button_Queue = xQueueCreate(4, 6);
    if (Button_Queue == NULL)
    {
        printf(" Crate: Failed to Button_Queue\n");
    }

    xTaskCreate((TaskFunction_t)start_task,
                (const char *)"start_task",
                (uint16_t)50,
                (void *)NULL,
                (UBaseType_t)30,
                (TaskHandle_t *)&StartTask_Handler);

    xTaskCreate((TaskFunction_t)Button_task,
                (const char *)"Button_task",
                (uint16_t)50,
                (void *)NULL,
                (UBaseType_t)10,
                (TaskHandle_t *)&ButtonTask_Handler);

    vTaskStartScheduler();
}

void start_task(void)
{
    BaseType_t Queue_state = pdFALSE;
    uint16_t Queue_buff[10] = {0};
    while (1)
    {
        AT32_LEDn_Toggle(LED4);
        // Delay_ms(1000);
        vTaskDelay(10);
        AT32_LEDn_Toggle(LED3);
        // Delay_ms(1000);
        vTaskDelay(100);
        //AT32_LEDn_Toggle(LED2);
        //Delay_ms(500);
        //printf("test!!\n");
        if (Button_Queue != NULL)
        {
            Queue_state = xQueueReceive(Button_Queue, Queue_buff, 10);
            if (Queue_state == pdTRUE)
            {
                printf("Queue_buff[0] = %d\n", Queue_buff[0]);
                printf("Queue_buff[1] = %d\n", Queue_buff[1]);
                printf("Queue_buff[2] = %d\n", Queue_buff[2]);
                printf("Queue_buff[3] = %d\n", Queue_buff[3]);
                printf("Queue_buff[4] = %d\n", Queue_buff[4]);
                printf("Queue_buff[5] = %d\n", Queue_buff[5]);
                printf("Queue_buff[6] = %d\n", Queue_buff[6]);
                printf("Queue_buff[7] = %d\n", Queue_buff[7]);
                printf("Queue_buff[8] = %d\n", Queue_buff[8]);
                printf("Queue_buff[9] = %d\n", Queue_buff[9]);
            }
            else
            {
                //vTaskDelay(100);
            }
        }
    }
}
extern uint8_t USART1_END, intrupt_NUM, USART_INT_RDNE_NUM, USART_INT_IDLEF_NUM, USART_INT_TRAC_NUM;
extern uint8_t RxBuffer1[100];
void Button_task(void)
{
    BaseType_t Queue_state = pdFALSE;
    uint16_t test = 128;
    while (1)
    {
        switch (AT32_BUTTON_Press())
        {
        case 0:
            /*            AT32_LEDn_Toggle(LED2);
             printf("USART Printf Example: retarget the C library printf function to the USART\n\r");
           */
            if (Button_Queue != NULL)
            {
                Queue_state = xQueueSend(Button_Queue, &test, 1);
                if (Queue_state != pdTRUE)
                {
                    printf("Queue send fail -_-!!\n");
                }
            }
            printf(" %d %d %d %d", intrupt_NUM, USART_INT_RDNE_NUM, USART_INT_IDLEF_NUM, USART_INT_TRAC_NUM);
            printf("RxBuffer1:%s\n", RxBuffer1);
            break;
        case 1:
            break;
        default:
            break;
        }
        if (Button_Queue != NULL)
        {
        }
        if (USART1_END == 0)
        {
            printf("RxBuffer1:%s", RxBuffer1);
            USART1_END = 1;
            if (strstr((char*)RxBuffer1, "LED_ON"))
            {
                AT32_LEDn_ON(LED2);
            }
            if (strstr((char*)RxBuffer1, "LED_OFF"))
            {
                AT32_LEDn_OFF(LED2);
            }
            memset(RxBuffer1, 0, sizeof(RxBuffer1));
        }
    }
}
