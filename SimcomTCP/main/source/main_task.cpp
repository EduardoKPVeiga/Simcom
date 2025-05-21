#include "main_task.h"
#include "pins.h"

#define TAG "main"

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
    string sn = "S1mC0M5";
    pins_init();
    Simcom simcom = Simcom(sn);
    simcom.power(true);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    main_task_send_message(START_MQTT);

    for (;;)
    {
        if (xQueueReceive(main_task_queue_handle, &msg, portMAX_DELAY))
        {
            switch (msg.msgID)
            {

            case START_NETWORK:
                ESP_LOGI(TAG, "START_NETWORK");
                if (!simcom.start_network())
                {
                    ESP_LOGE(TAG, "Start network failed.");
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    main_task_send_message(START_NETWORK);
                }
                else
                    main_task_send_message(START_MQTT);
                break;

            case START_MQTT:
                ESP_LOGI(TAG, "START_MQTT");
                if (!simcom.mqtt_connect("172.104.199.107", 1883))
                {
                    ESP_LOGE(TAG, "Start MQTT failed.");
                    simcom.close_connection();
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    main_task_send_message(START_MQTT);
                }
                else
                    main_task_send_message(SEND_MSG);
                break;

            case SEND_MSG:
                ESP_LOGI(TAG, "SEND_MSG");
                if (simcom.mqtt_send_msg("S1mC0M5/2", "teste"))
                {
                    vTaskDelay(5000 / portTICK_PERIOD_MS);
                    main_task_send_message(SEND_MSG);
                }
                else
                    main_task_send_message(RESTART_DEVICE);
                break;

            case RESTART_DEVICE:
                simcom.power(false);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                esp_restart();
                break;

            case SIMCOM_PRW_ON:
                ESP_LOGW(TAG, "SIMCOM_PRW_ON");
                simcom.power(true);
                xQueueReset(main_task_queue_handle);
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                main_task_send_message(START_NETWORK);
                break;

            default:
                break;
            }
        }
    }
}
