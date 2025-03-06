#include "SimcomUart.h"

using namespace mtw_str;

QueueHandle_t SimcomUart::uart_queue = NULL;
TaskHandle_t SimcomUart::uartGsmTaskHandle = NULL;
volatile bool SimcomUart::received = false;
uart_port_t SimcomUart::uart_num = UART_NUM_2;
SimcomRespList SimcomUart::simcom_resp_list = SimcomRespList();

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
    uart_set_pin(uart_num, PIN_SINCOM_TX, PIN_SINCOM_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void SimcomUart::close()
{
    uart_set_pin(uart_num, GPIO_NUM_NC, GPIO_NUM_NC, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    set_pin(PIN_SINCOM_RX, GPIO_MODE_INPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_SINCOM_TX, GPIO_MODE_INPUT, GPIO_INTR_DISABLE, NULL, NULL);
}

void SimcomUart::send(const char *data, size_t size)
{
    uart_write_bytes(uart_num, data, size);
    uart_wait_tx_done(uart_num, 100);
}

void SimcomUart::simcom_uart_task(void *pvParameters)
{
    cout << "simcom_uart_task" << endl;
    uart_event_t event;
    bool big_receive = false;
    char raw_msg_received[MSG_RECEIVED_BUFF_SIZE] = {0};
    string last_msg = "";
    string msg = "";
    size_t size = 0;

    for (;;)
    {
        if (xQueueReceive(uart_queue, (void *)&event, portMAX_DELAY))
        {
            switch (event.type)
            {
            case UART_DATA:
                size = event.size;
                uart_read_bytes(uart_num, raw_msg_received, size, portMAX_DELAY);

                if (mtw_str::StrContainsSubstr(raw_msg_received, SMSUB, size, SIZE(SMSUB)) >= 0 && mtw_str::StrContainsSubstr(raw_msg_received, BEGIN_CMD, size, SIZE(BEGIN_CMD)) < 0)
                {
                    smsub_received(raw_msg_received, size);
                }
                else
                {
                    mtw_str::remove_char(raw_msg_received, &size, '\0');
                    mtw_str::remove_char(raw_msg_received, &size, '\r');
                    msg = string(raw_msg_received, size);
                    msg_received(msg, size, &big_receive, &last_msg);
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

void SimcomUart::smsub_received(char *msg, size_t size)
{
    int index_msg = 0;
    int index_topic = 0;
    size_t msg_pub_size = 0;
    char slot = '0';

    index_topic = mtw_str::StrContainsChar(msg, '"', (int)size); // [+SMSUB: "topic","msg_pub"]
    if (index_topic < 0)
        return;
    index_topic += 1;
    slot = msg[index_topic + 8];

    index_msg = mtw_str::StrContainsChar(msg, ',', (int)size);
    if (index_msg < 0)
        return;
    msg_pub_size = size - index_msg - 5; // [+SMSUB: "topic","msg_pub"], timestamp
    index_msg += 2;

    char buff[msg_pub_size] = {'\0'};
    int j = 0;
    for (int i = 0; i < msg_pub_size; i++)
    {
        buff[j] = msg[i + index_msg];
        j++;
    }
    string log = mtw_str::to_hex_string(buff, msg_pub_size);

    if (slot == '0')
        ;
    // decode_message(buff, msg_pub_size);
}

void SimcomUart::msg_received(string msg, size_t size, bool *big_receive, string *last_msg)
{
    string msg_aux = msg;
    size_t size_aux = size;
    if (msg.find(APP_PDP) != std::string::npos || msg.find(RESP_SMS_READY) != std::string::npos || msg.find(CGNSPWR) != std::string::npos || msg.find(SGNSCMD) != std::string::npos || msg.find(SGNSERR) != std::string::npos || msg.find(SMCONN_RESP_TRUE) != std::string::npos || msg.find(SMS_READY) != std::string::npos)
    {
        received = true;
    }
    else if (!msg.find('>'))
    {
        if (!msg.find(RESP_OK) && !msg.find(RESP_ERROR))
        {
            (*big_receive) = true;
            received = false;
            (*last_msg) = msg;
        }
        else
        {
            if (*big_receive)
            {
                msg_aux = (*last_msg) + msg;
                size_aux = (*last_msg).size() + size;
            }
            received = true;
            (*big_receive) = false;
        }
    }
    else
    {
        received = true;
        (*big_receive) = false;
        msg_aux = ">";
        string from = "AT+SMPUB=";
        uint16_t size_smpub = msg_aux.size() + from.size();
        simcom_resp_list.add(SimcomResp(msg_aux, from, size_smpub));
        return;
    }

    if (received)
    {
        simcom_resp_list.add(SimcomResp(msg_aux, size_aux));
        received = false;
    }
}

SimcomResp SimcomUart::get_resp(Command cmd)
{
    SimcomResp resp = SimcomResp();
    if (!simcom_resp_list.empty())
        resp = simcom_resp_list.get(string(cmd.cmd));
    return resp;
}