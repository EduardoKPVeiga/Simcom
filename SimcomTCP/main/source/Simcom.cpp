#include "Simcom.h"

Simcom::Simcom() : simcomUart(SimcomUart()), lte(Lte())
{
    simcomUart.open();
}

Simcom::Simcom(unsigned char sn[SN_SIZE]) : simcomUart(SimcomUart()), lte(Lte())
{
    memcpy(serial_num, sn, SN_SIZE);
    simcomUart.open();
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

bool Simcom::connect()
{
    return lte.connect();
}

bool Simcom::disconnect()
{
    return lte.disconnect();
}

bool Simcom::connected()
{
    return lte.connected();
}