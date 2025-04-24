#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "Simcom.h"
#include "mtw_str.h"
#include "MqttConPacket.h"
#include "MqttPubPacket.h"
#include "MqttSubPacket.h"

// Main application task
#define MAIN_TASK_STACK_SIZE 10 * 1024
#define MAIN_TASK_PRIORITY configMAX_PRIORITIES - 2
#define MAIN_TASK_CORE_ID 0

/*
 * Message IDs for the WiFi application task
 */
typedef enum main_task_message
{
    START_NETWORK,
    START_MQTT,
    SEND_MSG,
    RESTART_DEVICE,
    SIMCOM_PRW_ON
} main_task_message_e;

typedef struct main_task_queue_message
{
    main_task_message_e msgID;

} main_task_queue_message_t;

BaseType_t main_task_send_message(main_task_message_e msgID);

BaseType_t main_task_send_message_from_isr(main_task_message_e msgID);

void main_task_init(void);

void main_task(void *pvParameters);

bool start_network(Simcom *simcom);

bool start_mqtt(Simcom *simcom);

bool send_msg(Simcom *simcom);

#endif // MAIN_TASK_H