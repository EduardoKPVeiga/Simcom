#include "Simcom.h"

Simcom::Simcom() : simcomUart(SimcomUart()), lte(Lte())
{
}

Simcom::Simcom(unsigned char sn[SN_SIZE]) : simcomUart(SimcomUart()), lte(Lte())
{
    memcpy(serial_num, sn, SN_SIZE);
}

Simcom::~Simcom()
{
}

bool Simcom::power(bool pwr)
{
    bool v = true;
    simcomUart.close();
    v = pwr ? pwrkey_power_on() : pwrkey_power_off();
    simcomUart.open();
    return v;
}