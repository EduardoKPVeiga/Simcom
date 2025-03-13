#include "SimcomUart.h"

#define TAG "SimcomUart"

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
    uart_set_pin(uart_num, PIN_SIMCOM_TX, PIN_SIMCOM_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void SimcomUart::close()
{
    uart_set_pin(uart_num, GPIO_NUM_NC, GPIO_NUM_NC, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    set_pin(PIN_SIMCOM_RX, GPIO_MODE_INPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_SIMCOM_TX, GPIO_MODE_INPUT, GPIO_INTR_DISABLE, NULL, NULL);
}

void SimcomUart::send(const char *data, size_t size)
{
    uart_write_bytes(uart_num, data, size);
    uart_wait_tx_done(uart_num, 100);
}

void SimcomUart::simcom_uart_task(void *pvParameters)
{
    uart_event_t event;
    bool big_receive = false;
    char raw_msg_received[MSG_RECEIVED_BUFF_SIZE] = {0};
    string last_msg = "";
    string msg = "";
    size_t total_size = 0;
    size_t event_size = 0;

    for (;;)
    {
        if (xQueueReceive(uart_queue, (void *)&event, portMAX_DELAY))
        {
            switch (event.type)
            {
            case UART_DATA:
                event_size = event.size;
                uart_read_bytes(uart_num, raw_msg_received, event_size, portMAX_DELAY);
                mtw_str::remove_char(raw_msg_received, &event_size, '\0');
                mtw_str::remove_char(raw_msg_received, &event_size, '\r');
                msg = string(raw_msg_received, event_size);
                msg_received(msg, &total_size, event_size, &last_msg);
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

void SimcomUart::msg_received(string msg, size_t *total_size, size_t size, string *last_msg)
{
    if (msg.contains(SGNSERR) || msg.contains(SMS_READY) || msg.contains(NORMAL_POWER_DOWN))
    {
        SimcomResp resp = SimcomResp(msg, size);
        simcom_resp_list.add(resp);
        *last_msg = "";
        *total_size = 0;
        return;
    }

    if (msg.contains("AT+"))
    {
        *last_msg = msg;
        *total_size = size;
        if (msg.contains(RESP_OK) || msg.contains(RESP_ERROR))
        {
            SimcomResp resp = SimcomResp(msg, size);
            simcom_resp_list.add(resp);
            *last_msg = "";
            *total_size = 0;
            return;
        }
    }
    else
    {
        if ((*last_msg).contains("AT+"))
        {
            *last_msg += msg;
            *total_size += size;
            if (msg.contains(RESP_OK) || msg.contains(RESP_ERROR))
            {
                SimcomResp resp = SimcomResp(*last_msg, *total_size);
                simcom_resp_list.add(resp);
                *last_msg = "";
                *total_size = 0;
                return;
            }
        }
        else if (msg.contains('+'))
        {
            SimcomResp resp = SimcomResp(msg, size);
            simcom_resp_list.add(resp);
            *last_msg = "";
            *total_size = 0;
            // cout << "3===========================" << endl
            //      << "---------- From " << endl
            //      << resp.from << endl
            //      << "---------- Received" << endl
            //      << resp.msg << endl
            //      << "============================" << endl;
            return;
        }
    }
}

SimcomResp SimcomUart::get_resp(Command cmd)
{
    SimcomResp resp = SimcomResp();
    if (!simcom_resp_list.empty())
        resp = simcom_resp_list.get(string(cmd.cmd));
    return resp;
}