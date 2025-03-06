#include "Casend.h"

Casend::Casend() : Command(), data({0}), data_size(0)
{
}

Casend::Casend(const char *cmd, CMD_action_enum action, char *data, uint16_t data_size) : Command(cmd, action), data({0}), data_size(data_size)
{
    memcpy(this->data, data, data_size);
}

Casend::~Casend()
{
}