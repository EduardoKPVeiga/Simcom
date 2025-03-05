#include "main_task.h"

// Queue handle used to manipulate the main queue of events
static QueueHandle_t main_task_queue_handle;

BaseType_t main_task_send_message(main_task_message_e msgID)
{
    main_task_queue_message_t msg;
    msg.msgID = msgID;
    return xQueueSend(main_task_queue_handle, &msg, portMAX_DELAY);
}

BaseType_t main_task_send_message_from_isr(main_task_message_e msgID)
{
    main_task_queue_message_t msg;
    msg.msgID = msgID;
    return xQueueSendFromISR(main_task_queue_handle, &msg, NULL);
}

void main_task_init(void)
{
    main_task_queue_handle = xQueueCreate(200, sizeof(main_task_queue_message_t));
    xTaskCreatePinnedToCore(&main_task, "main_task", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL, MAIN_TASK_CORE_ID);
}

void main_task(void *pvParameters)
{
    main_task_queue_message_t msg;
    pins_init();
    for (;;)
    {
        if (xQueueReceive(main_task_queue_handle, &msg, portMAX_DELAY))
        {
            switch (msg.msgID)
            {

            case START_MQTT:
                break;

            case RESTART_DEVICE:
                break;

            case UART_DATA_RECEIVED:
                break;

            case SS_ISR:
                break;

            default:
                break;
            }
        }
    }
}