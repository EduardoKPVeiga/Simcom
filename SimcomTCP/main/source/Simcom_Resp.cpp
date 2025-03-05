#include "Simcom_Resp.h"

static const char *TAG = "Simcom_Resp";

SemaphoreHandle_t simcom_resp_semaphore = NULL;
queue<simcom_resp::Simcom_Resp> resp_queue;

using namespace simcom_resp;

void simcom_resp::init_simcom_resp_semaphore()
{
    simcom_resp_semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(simcom_resp_semaphore);
}

Simcom_Resp::Simcom_Resp() : from(""),
                             msg(""),
                             size(0),
                             not_used(0) {}

Simcom_Resp::Simcom_Resp(string _msg, string _from, uint16_t _size) : from(_from),
                                                                      msg(_msg),
                                                                      size(_size),
                                                                      not_used(0) {}

Simcom_Resp::~Simcom_Resp() {}

bool simcom_resp::queue_empty()
{
    bool value = true;
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        value = resp_queue.empty();
        xSemaphoreGive(simcom_resp_semaphore);
    }
    return value;
}

uint16_t simcom_resp::queue_size()
{
    if (queue_empty())
        return 0;

    uint16_t value = 0;
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        value = resp_queue.size();
        xSemaphoreGive(simcom_resp_semaphore);
    }
    return value;
}

void simcom_resp::clear_queue()
{
    if (simcom_resp_semaphore == NULL)
        return;

    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        while (!resp_queue.empty())
            resp_queue.pop();
        xSemaphoreGive(simcom_resp_semaphore);
    }
}

void simcom_resp::enqueue(string _msg, string _from, uint16_t _size)
{
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        Simcom_Resp item = Simcom_Resp(_msg, _from, _size);

        if (resp_queue.size() >= MAX_RESP_NUM)
            resp_queue.pop();
        resp_queue.push(item);

        xSemaphoreGive(simcom_resp_semaphore);
    }
}

void simcom_resp::enqueue(string msg, uint16_t size)
{
    int begin = msg.find(BEGIN_CMD);
    if (begin >= 0)
        enqueue(extract_msg(&(msg[begin]), size), extract_from(&(msg[begin]), size), size);
    else
    {
        begin = msg.find('+');
        if (begin >= 0)
            enqueue(extract_msg(&(msg[begin]), size), extract_from(&(msg[begin]), size), size);
        else
            enqueue(extract_msg(msg, size), extract_from(msg, size), size);
    }
}

Simcom_Resp simcom_resp::dequeue()
{
    Simcom_Resp item = Simcom_Resp();
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        item = resp_queue.front();
        resp_queue.pop();
        xSemaphoreGive(simcom_resp_semaphore);
    }
    return item;
}

Simcom_Resp simcom_resp::get_item(string cmd)
{
    queue<Simcom_Resp> aux_queue;
    Simcom_Resp item = Simcom_Resp();

    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        while (!resp_queue.empty())
        {
            resp_queue.front().not_used += 1;
            if (((resp_queue.front()).from).find(cmd) != std::string::npos)
                item = resp_queue.front();
            else
            {
                if (resp_queue.front().not_used <= MAX_NOT_USED_NUM)
                    aux_queue.push(resp_queue.front());
            }
            resp_queue.pop();
        }

        while (!aux_queue.empty())
        {
            resp_queue.push(aux_queue.front());
            aux_queue.pop();
        }
        xSemaphoreGive(simcom_resp_semaphore);
    }
    return item;
}

Simcom_Resp simcom_resp::get_item(string cmd, string msg)
{
    queue<Simcom_Resp> aux_queue;
    Simcom_Resp item = Simcom_Resp();

    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        while (!resp_queue.empty())
        {
            resp_queue.front().not_used += 1;
            if (((resp_queue.front()).from).find(cmd) != std::string::npos && ((resp_queue.front()).msg).find(msg) != std::string::npos)
                item = resp_queue.front();
            else
            {
                if (resp_queue.front().not_used <= MAX_NOT_USED_NUM)
                    aux_queue.push(resp_queue.front());
            }
            resp_queue.pop();
        }

        while (!aux_queue.empty())
        {
            resp_queue.push(aux_queue.front());
            aux_queue.pop();
        }
        xSemaphoreGive(simcom_resp_semaphore);
    }
    return item;
}

string simcom_resp::extract_from(string msg, uint16_t size)
{
    string from = "";
    int index = 0;
    int begin_cmd_index = -1;

    begin_cmd_index = msg.find(BEGIN_CMD);
    if (begin_cmd_index >= 0)
        index = msg.find('\n', begin_cmd_index + SIZE(BEGIN_CMD)) + 1;
    else
    {
        index = msg.find(':') + 1;
        if (index < 1)
            index = 0;
    }

    for (int i = 0; i < index; i++)
    {
        if (msg[i] != '\r' && msg[i] != '\0')
            from += msg[i];
    }
    from.erase(remove(from.begin(), from.end(), '\r'), from.end());
    from.erase(remove(from.begin(), from.end(), '\n'), from.end());
    return from;
}

string simcom_resp::extract_msg(string msg, uint16_t size)
{
    string msg_str = "";
    int i = 0;
    int begin_cmd_index = -1;

    begin_cmd_index = msg.find(BEGIN_CMD);
    if (begin_cmd_index >= 0)
        i = msg.find('\n', begin_cmd_index + SIZE(BEGIN_CMD)) + 1;
    else
    {
        i = msg.find(':') + 1;
        if (i < 1)
            i = 0;
    }

    for (; i < size; i++)
    {
        if (msg[i] != '\r' && msg[i] != '\0')
            msg_str += msg[i];
    }
    msg_str.erase(remove(msg_str.begin(), msg_str.end(), '\r'), msg_str.end());
    msg_str.erase(remove(msg_str.begin(), msg_str.end(), '\n'), msg_str.end());
    return msg_str;
}

string simcom_resp::c_array_to_string(char *c_array, uint16_t size)
{
    string msg = "";
    for (int i = 0; i < size; i++)
        msg += c_array[i];
    return msg;
}