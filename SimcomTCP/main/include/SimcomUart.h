#ifndef SIMCOMUART_H
#define SIMCOMUART_H

#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "pins.h"
#include <string>
#include <queue>

#include "simcom_definitions.h"
#include "mtw_str.h"
#include "Simcom_Resp.h"

using namespace std;

#define BAUD_RATE 38400
#define MSG_RECEIVED_BUFF_SIZE 4 * 1024

#define SIMCOM_UART_TASK_STACK_SIZE 16 * 1024
#define SIMCOM_UART_TASK_PRIORITY configMAX_PRIORITIES - 1
#define SIMCOM_UART_TASK_CORE_ID 0

class SimcomUart
{
private:
    static QueueHandle_t uart_queue;
    static TaskHandle_t uartGsmTaskHandle;

    static void config();
    static void simcom_uart_task(void *pvParameters);
    static void smsub_received(char *msg, size_t size);
    static void msg_received(string msg, size_t size, bool *big_receive, string *last_msg);

public:
    static volatile bool received;
    static uart_port_t uart_num;
    SimcomUart();
    ~SimcomUart();
    static void open();
    static void close();
    static void send(const char *data);
};

#endif // SIMCOMUART_H