#ifndef SIMCOMRESPLIST_H
#define SIMCOMRESPLIST_H

#include "SimcomResp.h"
#include <list>

class SimcomRespList
{
private:
public:
    uint16_t size;
    list<SimcomResp> simcom_resp_list;
    SemaphoreHandle_t simcom_resp_semaphore;

    SimcomRespList();
    ~SimcomRespList();

    bool empty();
    void clear();
    SimcomResp remove(string cmd);
    SimcomResp get(string cmd);
    SimcomResp get(string cmd, string msg);
    void add(SimcomResp item);
};

#endif // SIMCOMRESPLIST_H