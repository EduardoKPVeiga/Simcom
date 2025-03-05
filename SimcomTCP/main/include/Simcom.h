#ifndef SIMCOM_H
#define SIMCOM_H

#include "Lte.h"
#include "SimcomUart.h"

#define SN_SIZE 7

class Simcom
{
private:
    SimcomUart simcomUart;
    Lte lte;
    char serial_num[SN_SIZE];

public:
    Simcom();
    Simcom(unsigned char sn[SN_SIZE]);
    ~Simcom();

    bool init();
    bool power(bool pwr);
    bool connect();
    bool disconnect();
    bool connected();
};

#endif // SIMCOM_H