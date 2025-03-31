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
    main_task_send_message(START_NETWORK);

    for (;;)
    {
        if (xQueueReceive(main_task_queue_handle, &msg, portMAX_DELAY))
        {
            switch (msg.msgID)
            {

            case START_NETWORK:
                ESP_LOGI(TAG, "START_NETWORK");
                if (!start_network(&simcom))
                    main_task_send_message(START_NETWORK_ERROR);
                else
                    main_task_send_message(START_MQTT);
                break;

            case START_NETWORK_ERROR:
                ESP_LOGE(TAG, "Start network failed.");
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                main_task_send_message(START_NETWORK);
                break;

            case START_MQTT:
                ESP_LOGI(TAG, "START_MQTT");
                if (!start_mqtt(&simcom))
                    main_task_send_message(START_MQTT_ERROR);
                else
                    // main_task_send_message(SEND_MSG);
                    ESP_LOGI(TAG, "openned");
                break;

            case START_MQTT_ERROR:
                ESP_LOGE(TAG, "Start MQTT failed.");
                // start_mqtt_error_routine(&simcom);
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
    int index = resp.msg.find(':');
    if (!resp.valid(cmd) || (resp.msg[index + 2] == '9' && resp.msg[index + 3] == '9'))
        return false;

    cmd = Command(CGNAPN, CMD_action_enum::EXE);
    resp = (*simcom).send(cmd);
    if (!resp.valid(cmd))
        return false;
    else
    {
        index = resp.msg.find(':');
        if (resp.msg[index + 2] == '0')
            return false;
    }

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
        return false;

    cmd = Command(CNACT, CMD_action_enum::READ);
    resp = (*simcom).send(cmd);
    index = resp.msg.find(':') + 5;
    if (resp.msg[index] == '0')
        return false;
    else if (resp.msg[index] == '1')
        return true;
    else
    {
        vTaskDelay(6000 / portTICK_PERIOD_MS);
        cmd = Command(APP_PDP, CMD_action_enum::EXE);
        resp = (*simcom).get_resp(cmd);
        if (resp.valid(cmd))
        {
            if (!resp.msg.contains("DEACTIVE"))
                return true;
            return false;
        }
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
    resp = (*simcom).send(cmd, 2000);
    if (!resp.valid(cmd))
        return false;

    MqttCon con_packet = MqttCon("S1mC0M5");
    con_packet.build();
    Casend casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, (char *)con_packet.buffer(), con_packet.buffer_size());
    cmd = (Command)casend_cmd;
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)con_packet.buffer_size()));
    resp = (*simcom).send(casend_cmd);
    if (!resp.valid(cmd))
        return false;

    // MqttSub sub_packet = MqttSub(1564);
    // sub_packet.add_topic("S1mC0M5/2", qos_e::AT_MOST_ONCE);
    // sub_packet.build();
    // casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, (char *)sub_packet.buffer(), sub_packet.buffer_size());
    // cmd = (Command)casend_cmd;
    // casend_cmd.add_value(Value((int)0));
    // casend_cmd.add_value(Value((int)sub_packet.buffer_size()));
    // resp = (*simcom).send(casend_cmd);
    // if (!resp.valid(cmd))
    //     return false;

    return true;
}

bool send_msg(Simcom *simcom)
{
    uint8_t data[5] = {'H', 'E', 'L', 'L', 'O'};
    MqttPub pub_packet = MqttPub(1564);
    pub_packet.set("S1mC0M5/1", data, 5);
    pub_packet.build();
    Casend casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, (char *)pub_packet.buffer(), pub_packet.buffer_size());
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)pub_packet.buffer_size()));
    SimcomResp resp = (*simcom).send(casend_cmd);
    return resp.valid(casend_cmd);
}

void start_mqtt_error_routine(Simcom *simcom)
{
    Command cmd = Command(CASTATE, CMD_action_enum::EXE);
    SimcomResp resp = (*simcom).get_resp(cmd);
    int index = resp.msg.find(',');
    if (!resp.valid(cmd) || resp.msg[index + 1] != '0')
    {
        cmd = Command(CACLOSE, CMD_action_enum::WRITE);
        cmd.add_value(Value((int)0));
        (*simcom).send(cmd);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    cmd = Command(CNACT, CMD_action_enum::READ);
    resp = (*simcom).send(cmd);
    index = resp.msg.find(':') + 5;
    if (resp.msg[index] == 0)
    {
        main_task_send_message(START_NETWORK);
    }
    else
    {
        main_task_send_message(RESTART_DEVICE);
    }
}