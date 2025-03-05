#include "Simcom.h"

Simcom::Simcom() : simcomUart(SimcomUart()), cmd_queue(SimcomCmdQueue(cmd_queue_type_e::EMPTY))
{
    simcomUart.open();
}

Simcom::Simcom(unsigned char sn[SN_SIZE]) : simcomUart(SimcomUart()), cmd_queue(SimcomCmdQueue(cmd_queue_type_e::EMPTY))
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