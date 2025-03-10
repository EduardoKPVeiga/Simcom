#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "pins.h"
#include "Simcom.h"
#include "mtw_str.h"
#include "MqttPacket.h"

// Main application task
#define MAIN_TASK_STACK_SIZE 10 * 1024
#define MAIN_TASK_PRIORITY configMAX_PRIORITIES - 2
#define MAIN_TASK_CORE_ID 0

/*
 * Message IDs for the WiFi application task
 */
typedef enum main_task_message
{
    START_MQTT,
    SEND_MSG,
    RESTART_DEVICE,
    UART_DATA_RECEIVED,
    SS_ISR
} main_task_message_e;

typedef struct main_task_queue_message
{
    main_task_message_e msgID;

} main_task_queue_message_t;

BaseType_t main_task_send_message(main_task_message_e msgID);

BaseType_t main_task_send_message_from_isr(main_task_message_e msgID);

void main_task_init(void);

void main_task(void *pvParameters);

SimcomCmdQueue create_start_mqtt_queue();

SimcomCmdQueue create_send_msg_queue();

#endif // MAIN_TASK_H