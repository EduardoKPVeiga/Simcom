#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "common.h"

/*
 * Message IDs for the WiFi application task
 */
typedef enum main_task_message
{
    WIFI_APP_MSG_START_HTTP_SERVER = 0,
    WIFI_APP_MSG_LOAD_SAVED_CREDENTIALS,
    BUTTON_FUNCTION_1,
    BUTTON_FUNCTION_2,
    BUTTON_FUNCTION_3,
    BUTTON_FUNCTION_4,
    BUTTON_FUNCTION_5,
    START_MODE_MASTER,
    START_MODE_SLAVE,
    START_MODE_LTE,
    START_DEEP_SLEEP,
    START_SENSORS,
    START_SENSORS_WIFI,
    START_MQTT,
    RESTART_DEVICE,
    RESTART_GSM,
    START_GNSS,
    START_EXPANSIONS,
    UART_DATA_RECEIVED,
    START_BROADCAST,
    SS_ISR,
    START_HTTP_SERVER,
    START_WIFI,
    START_NTP,
} main_task_message_e;

/*
 * Structure for the message queue
 */
typedef struct main_task_queue_message
{
    main_task_message_e msgID;

} main_task_queue_message_t;

/*
 * Sends a message to the queue
 * @author GCA
 * @param msgID message IS from the wifi_task_message_e enum.
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE.
 * @note Expand this based on application requirements e.g. how you've expanded the wifi_task_queue_message_t.
 */

BaseType_t main_task_send_message(main_task_message_e msgID);

/*
 * Sends a message to the queue from a ISR
 * @author GCA
 * @param msgID message IS from the wifi_task_message_e enum.
 * @return pdTRUE if an item was successfully sent to the queue, otherwise pdFALSE.
 * @note Expand this based on application requirements e.g. how you've expanded the wifi_task_queue_message_t.
 */

BaseType_t main_task_send_message_from_isr(main_task_message_e msgID);

/*
 * Main task for the WiFi application
 * @author GCA
 * @param pvParameteres parameter which can be passed to the task
 */
void main_task(void *pvParameters);

/*
 * Initializes main task
 * @author GCA
 */
void main_task_init(void);

#endif // MAIN_TASK_H