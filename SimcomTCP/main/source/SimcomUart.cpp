#include "SimcomUart.h"

#define TAG "SimcomUart"

using namespace mtw_str;

QueueHandle_t SimcomUart::uart_queue = NULL;
TaskHandle_t SimcomUart::uartGsmTaskHandle = NULL;
volatile bool SimcomUart::received = false;
uart_port_t SimcomUart::uart_num = UART_NUM_2;
SimcomRespList SimcomUart::simcom_resp_list = SimcomRespList();
queue<Caurc> server_msg_queue = queue<Caurc>();

SimcomUart::SimcomUart()
{
    config();
    xTaskCreatePinnedToCore(&simcom_uart_task, "uart GSM sim task", SIMCOM_UART_TASK_STACK_SIZE, (void *)uart_num, SIMCOM_UART_TASK_PRIORITY, &uartGsmTaskHandle, SIMCOM_UART_TASK_CORE_ID);
}

SimcomUart::~SimcomUart()
{
    close();
}

void SimcomUart::config()
{
    uart_config_t uart_config_gsm = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_driver_install(uart_num, MSG_RECEIVED_BUFF_SIZE, MSG_RECEIVED_BUFF_SIZE, 10, &uart_queue, 0);
    uart_param_config(uart_num, &uart_config_gsm);
}

void SimcomUart::open()
{
    uart_set_pin(uart_num, PIN_SIMCOM_TX, PIN_SIMCOM_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void SimcomUart::close()
{
    uart_set_pin(uart_num, GPIO_NUM_NC, GPIO_NUM_NC, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    set_pin(PIN_SIMCOM_RX, GPIO_MODE_INPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_SIMCOM_TX, GPIO_MODE_INPUT, GPIO_INTR_DISABLE, NULL, NULL);
}

void SimcomUart::send(Command cmd)
{
    uart_write_bytes(uart_num, cmd.msg_send, cmd.size);
    uart_wait_tx_done(uart_num, 100);
    vTaskDelay(DEFAULT_CMD_SEND_DELAY);
}

void SimcomUart::send(Casend casend)
{
    send((Command)casend);
    send(casend.data, casend.data_size);
}

void SimcomUart::send(const char *data, size_t size)
{
    uart_write_bytes(uart_num, data, size);
    uart_wait_tx_done(uart_num, 100);
    vTaskDelay(DEFAULT_CASEND_CMD_SEND_DELAY);
}

void SimcomUart::simcom_uart_task(void *pvParameters)
{
    uart_event_t event;
    char raw_msg_received[MSG_RECEIVED_BUFF_SIZE] = {0};
    list<string> msg_list = list<string>();
    string msg = "";
    int index = 0;
    bool waiting = false;
    Caurc server_msg = Caurc();

    for (;;)
    {
        if (xQueueReceive(uart_queue, (void *)&event, portMAX_DELAY))
        {
            switch (event.type)
            {
            case UART_DATA:
                uart_read_bytes(uart_num, raw_msg_received, event.size, portMAX_DELAY);
                index = mtw_str::StrContainsChar(raw_msg_received, '\0', event.size);

                if (mtw_str::StrContainsSubstr(raw_msg_received, CAURC, event.size, SIZE(CAURC)) >= 0)
                {
                    server_msg = Caurc(raw_msg_received, event.size);
                    cout << "Server Message: " << endl
                         << mtw_str::to_hex_string(server_msg.data, server_msg.length) << endl
                         << endl;
                    break;
                }

                if (index >= 0)
                {
                    msg = string(raw_msg_received, index);
                    msg_list.push_back(msg);
                    if (index + 1 < event.size - 3)
                    {
                        msg = string(&raw_msg_received[index + 1], event.size - index - 1);
                        msg_list.push_back(msg);
                    }
                    else
                        waiting = true;
                }
                else
                {
                    msg = string(raw_msg_received, event.size);
                    msg_list.push_back(msg);
                    if (waiting && (msg.contains(RESP_OK) || msg.contains(RESP_ERROR)))
                        waiting = false;
                }
                if (!waiting)
                {
                    simcom_resp_list.add(SimcomResp(msg_list));
                    msg_list.clear();
                }
                break;

            case UART_FIFO_OVF:
                uart_flush_input(uart_num);
                xQueueReset(uart_queue);
                break;

            case UART_BUFFER_FULL:
                uart_flush_input(uart_num);
                xQueueReset(uart_queue);
                break;

            case UART_BREAK:
                break;

            case UART_PARITY_ERR:
                break;

            case UART_FRAME_ERR:
                break;

            default:
                break;
            }
        }
    }
    vTaskDelete(NULL);
}

SimcomResp SimcomUart::get_resp(Command cmd)
{
    SimcomResp resp = SimcomResp();
    if (!simcom_resp_list.empty())
    {
        resp = simcom_resp_list.get(string(cmd.cmd));
        simcom_resp_list.remove(string(cmd.cmd));
    }
    return resp;
}