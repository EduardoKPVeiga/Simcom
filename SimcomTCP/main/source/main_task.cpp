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
    unsigned char sn[SN_SIZE] = {'S', '1', 'm', 'C', '0', 'M', '5'};
    pins_init();
    Simcom simcom = Simcom(sn);
    simcom.power(true);

    main_task_send_message(START_MQTT);

    for (;;)
    {
        if (xQueueReceive(main_task_queue_handle, &msg, portMAX_DELAY))
        {
            switch (msg.msgID)
            {

            case START_MQTT:
                ESP_LOGI(TAG, "START_MQTT");
                simcom.set_queue(create_start_mqtt_queue());
                if (!simcom.send())
                {
                    ESP_LOGE(TAG, "Error sending message.");
                    main_task_send_message(START_MQTT);
                }
                // else
                //     main_task_send_message(SEND_MSG);
                break;

            case SEND_MSG:
                ESP_LOGI(TAG, "SEND_MSG");
                simcom.set_queue(create_send_msg_queue());
                simcom.send();
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                main_task_send_message(SEND_MSG);
                break;

            case RESTART_DEVICE:
                simcom.power(false);
                esp_restart();
                break;

            case SIMCOM_PRW_ON:
                ESP_LOGW(TAG, "SIMCOM_PRW_ON");
                simcom.power(true);
                xQueueReset(main_task_queue_handle);
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                main_task_send_message(START_MQTT);
                break;

            default:
                break;
            }
        }
    }
}

SimcomCmdQueue create_start_mqtt_queue()
{
    SimcomCmdQueue queue = SimcomCmdQueue(cmd_queue_type_e::PDN_AUTO_ACT);

    Command cmd = Command(CPIN, CMD_action_enum::READ);
    queue.enqueue(cmd);

    cmd = Command(CSQ, CMD_action_enum::EXE);
    queue.enqueue(cmd);

    cmd = Command(CGATT, CMD_action_enum::READ);
    queue.enqueue(cmd);

    cmd = Command(COPS, CMD_action_enum::READ);
    queue.enqueue(cmd);

    cmd = Command(CGNAPN, CMD_action_enum::EXE);
    queue.enqueue(cmd);

    cmd = Command(CNCFG, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)1));
    cmd.add_value(Value("ctnb"));
    queue.enqueue(cmd);

    cmd = Command(CNACT, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)1));
    queue.enqueue(cmd);

    cmd = Command(CNACT, CMD_action_enum::READ);
    queue.enqueue(cmd);

    cmd = Command(CASSLCFG, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value("SSL"));
    cmd.add_value(Value((int)0));
    queue.enqueue(cmd);

    cmd = Command(CAOPEN, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)0));
    cmd.add_value(Value("TCP"));
    cmd.add_value(Value("172.104.199.107"));
    cmd.add_value(Value((int)1883));
    queue.enqueue(cmd);

    cmd = Command(CASTATE, CMD_action_enum::READ);
    queue.enqueue(cmd);

    MqttPacket packet = MqttPacket("S1mC0M5", "teste");
    packet.create_connect_packet("S1mC0M5", "user", "password");
    Casend casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, packet.buffer, packet.buffer_size);
    cmd = (Command)casend_cmd;
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)packet.buffer_size));
    queue.enqueue(cmd);
    queue.enqueue_casend(casend_cmd);

    cmd = Command(CARECV, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)4));
    queue.enqueue(cmd);

    cmd = Command(CACLOSE, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    queue.enqueue(cmd);

    return queue;
}

SimcomCmdQueue create_send_msg_queue()
{
    SimcomCmdQueue queue = SimcomCmdQueue(cmd_queue_type_e::EMPTY);

    Command cmd = Command(CAOPEN, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)0));
    cmd.add_value(Value("TCP"));
    cmd.add_value(Value("172.104.199.107"));
    cmd.add_value(Value((int)1883));
    queue.enqueue(cmd);

    cmd = Command(CASTATE, CMD_action_enum::READ);
    queue.enqueue(cmd);

    MqttPacket packet = MqttPacket("SNTESTE", "teste");
    packet.create_publish_packet();
    Casend casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, packet.buffer, packet.buffer_size);
    cmd = (Command)casend_cmd;
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)packet.buffer_size));
    queue.enqueue(cmd);
    queue.enqueue_casend(casend_cmd);

    cmd = Command(CACLOSE, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    queue.enqueue(cmd);

    return queue;
}