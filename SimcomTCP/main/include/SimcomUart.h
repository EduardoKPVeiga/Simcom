#ifndef SIMCOMUART_H
#define SIMCOMUART_H

#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "pins.h"
#include <string>
#include <queue>
#include "esp_log.h"

#include "simcom_definitions.h"
#include "mtw_str.h"
#include "SimcomRespList.h"
#include "Command.h"
#include "Casend.h"
#include "Caurc.h"

using namespace std;

#define BAUD_RATE 38400
#define MSG_RECEIVED_BUFF_SIZE 4 * 1024

#define SIMCOM_UART_TASK_STACK_SIZE 16 * 1024
#define SIMCOM_UART_TASK_PRIORITY configMAX_PRIORITIES - 1
#define SIMCOM_UART_TASK_CORE_ID 0

#define DEFAULT_CMD_SEND_DELAY 100 / portTICK_PERIOD_MS
#define DEFAULT_CASEND_CMD_SEND_DELAY 1000 / portTICK_PERIOD_MS

class SimcomUart
{
private:
    static QueueHandle_t uart_queue;
    static TaskHandle_t uartGsmTaskHandle;

    static void config();
    static void simcom_uart_task(void *pvParameters);

public:
    static volatile bool received;
    static uart_port_t uart_num;
    static SimcomRespList simcom_resp_list;
    static queue<Caurc> server_msg_queue;

    SimcomUart();
    ~SimcomUart();

    static void open();
    static void close();
    static void send(Command cmd);
    static void send(Casend cmd);
    static void send(const char *data, size_t size);
    static SimcomResp get_resp(Command cmd);
};

#endif // SIMCOMUART_H