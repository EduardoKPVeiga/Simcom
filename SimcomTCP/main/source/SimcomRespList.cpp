#include "SimcomRespList.h"

SimcomRespList::SimcomRespList() : size(0), simcom_resp_list(list<SimcomResp>())
{
    simcom_resp_semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(simcom_resp_semaphore);
}

SimcomRespList::~SimcomRespList()
{
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        simcom_resp_list.clear();
        xSemaphoreGive(simcom_resp_semaphore);
    }
}

bool SimcomRespList::empty()
{
    bool e = true;
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        e = simcom_resp_list.empty();
        xSemaphoreGive(simcom_resp_semaphore);
    }
    return e;
}

void SimcomRespList::clear()
{
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        simcom_resp_list.clear();
        size = 0;
        xSemaphoreGive(simcom_resp_semaphore);
    }
}

SimcomResp SimcomRespList::remove(string cmd)
{
    SimcomResp item = SimcomResp();
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        list<SimcomResp>::iterator it = simcom_resp_list.begin();
        while (it != simcom_resp_list.end())
        {
            if (it->from.find(cmd) != std::string::npos)
            {
                item = *it;
                simcom_resp_list.erase(it);
                size--;
                break;
            }
            it++;
        }
        xSemaphoreGive(simcom_resp_semaphore);
    }
    return item;
}

SimcomResp SimcomRespList::get(string cmd)
{
    SimcomResp item = SimcomResp();
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        list<SimcomResp>::iterator it = simcom_resp_list.begin();
        while (it != simcom_resp_list.end())
        {
            if (it->from.find(cmd) != std::string::npos)
            {
                item = *it;
                break;
            }
            it++;
        }
        xSemaphoreGive(simcom_resp_semaphore);
    }
    return item;
}

SimcomResp SimcomRespList::get(string cmd, string msg)
{
    SimcomResp item = SimcomResp();
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        list<SimcomResp>::iterator it = simcom_resp_list.begin();
        while (it != simcom_resp_list.end())
        {
            if (it->from.find(cmd) != std::string::npos && it->msg.find(msg) != std::string::npos)
            {
                item = *it;
                break;
            }
            it++;
        }
        xSemaphoreGive(simcom_resp_semaphore);
    }
    return item;
}

void SimcomRespList::add(SimcomResp item)
{
    if (xSemaphoreTake(simcom_resp_semaphore, portMAX_DELAY) == pdTRUE)
    {
        simcom_resp_list.push_back(item);
        size++;
        xSemaphoreGive(simcom_resp_semaphore);
    }
}