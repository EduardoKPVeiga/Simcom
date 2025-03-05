#ifndef SIMCOM_H
#define SIMCOM_H

// #include "Lte.h"
#include "SimcomUart.h"
#include "SimcomCmdQueue.h"

#define SN_SIZE 7

class Simcom
{
private:
    SimcomUart simcomUart;
    // Lte lte;
    SimcomCmdQueue cmd_queue;
    char serial_num[SN_SIZE];

public:
    Simcom();
    Simcom(unsigned char sn[SN_SIZE]);
    ~Simcom();

    bool power(bool pwr);
};

#endif // SIMCOM_H