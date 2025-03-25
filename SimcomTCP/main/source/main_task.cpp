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
                if (!start_network(&simcom))
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
                if (!start_mqtt(&simcom))
                {
                    ESP_LOGE(TAG, "Start MQTT failed.");
                    Command cmd = Command(CACLOSE, CMD_action_enum::WRITE);
                    cmd.add_value(Value((int)0));
                    simcom.send(cmd);
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    main_task_send_message(START_MQTT);
                }
                else
                    main_task_send_message(SEND_MSG);
                break;

            case SEND_MSG:
                ESP_LOGI(TAG, "SEND_MSG");
                if (send_msg(&simcom))
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
bool start_network(Simcom *simcom)
{
    SimcomResp resp = SimcomResp();
    Command cmd = Command();

    cmd = Command(CSQ, CMD_action_enum::EXE);
    resp = (*simcom).send(cmd);
    if (!resp.valid(cmd))
        return false;

    cmd = Command(CGNAPN, CMD_action_enum::EXE);
    resp = (*simcom).send(cmd);
    if (!resp.valid(cmd))
        return false;

    cmd = Command(CNCFG, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)1));
    cmd.add_value(Value("ctnb"));
    resp = (*simcom).send(cmd);
    if (!resp.valid(cmd))
        return false;

    cmd = Command(CNACT, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)1));
    resp = (*simcom).send(cmd);
    if (!resp.valid(cmd))
    {
        cmd = Command(CNACT, CMD_action_enum::READ);
        resp = (*simcom).send(cmd);
        int index = resp.msg.find(':') + 5;
        if (resp.msg[index] == 0)
            return false;
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    resp = (*simcom).get_resp(Command("+APP PDP", CMD_action_enum::EXE));
    if (resp.msg.contains("DEACTIVE"))
    {
        ESP_LOGE(TAG, "Network not activated.");
        return false;
    }
    return true;
}

bool start_mqtt(Simcom *simcom)
{
    SimcomResp resp = SimcomResp();
    Command cmd = Command();

    cmd = Command(CAOPEN, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)0));
    cmd.add_value(Value("TCP"));
    cmd.add_value(Value("172.104.199.107"));
    cmd.add_value(Value((int)1883));
    cmd.add_value(Value((int)1));
    resp = (*simcom).send(cmd);
    if (!resp.valid(cmd))
        return false;

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    MqttPacket packet = MqttPacket("S1mC0M5/1", "teste");
    packet.create_connect_packet("S1mC0M5");
    Casend casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, packet.buffer, packet.buffer_size);
    cmd = (Command)casend_cmd;
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)packet.buffer_size));
    resp = (*simcom).send(casend_cmd);
    if (!resp.valid(cmd))
        return false;

    packet = MqttPacket("S1mC0M5/2", "teste");
    packet.create_subscribe_packet(1564);
    casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, packet.buffer, packet.buffer_size);
    cmd = (Command)casend_cmd;
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)packet.buffer_size));
    resp = (*simcom).send(casend_cmd);
    if (!resp.valid(cmd))
        return false;

    return true;
}

bool send_msg(Simcom *simcom)
{
    MqttPacket packet = MqttPacket("S1mC0M5/1", "teste");
    packet.create_publish_packet();
    Casend casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, packet.buffer, packet.buffer_size);
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)packet.buffer_size));
    SimcomResp resp = (*simcom).send(casend_cmd);
    return resp.valid(casend_cmd);
}