#include "MqttSubVarHeader.h"

MqttSubVarHeader::MqttSubVarHeader(uint16_t id) : msg_id(id),
                                                  variable_header({0})
{
    variable_header[0] = id >> 8;
    variable_header[1] = id & 0x00FF;
}

MqttSubVarHeader::~MqttSubVarHeader()
{
}
