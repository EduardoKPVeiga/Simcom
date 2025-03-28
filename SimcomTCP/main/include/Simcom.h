#ifndef SIMCOM_H
#define SIMCOM_H

#include "SimcomUart.h"
#include "SimcomCmdQueue.h"
#include "simcom_definitions.h"

#define SN_SIZE 7

class Simcom
{
private:
    SimcomUart simcomUart;
    SimcomCmdQueue cmd_queue;
    char serial_num[SN_SIZE];

public:
    Simcom();
    Simcom(unsigned char sn[SN_SIZE]);
    ~Simcom();

    bool power(bool pwr);
    void set_queue(SimcomCmdQueue queue);
    SimcomResp send(Command cmd);
    SimcomResp send(Casend cmd);
    SimcomResp get_resp(Command cmd);
};

#endif // SIMCOM_H