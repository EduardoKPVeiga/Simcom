#include "MqttSubAckVarHeader.h"

MqttSubAckVarHeader::MqttSubAckVarHeader() : variable_header({0}), msg_id(0)
{
}

MqttSubAckVarHeader::MqttSubAckVarHeader(uint8_t v[2]) : MqttSubAckVarHeader()
{
    memcpy(this->variable_header, v, 2);
    this->msg_id = v[0] << 8;
    this->msg_id |= v[1];
}

MqttSubAckVarHeader::~MqttSubAckVarHeader()
{
}