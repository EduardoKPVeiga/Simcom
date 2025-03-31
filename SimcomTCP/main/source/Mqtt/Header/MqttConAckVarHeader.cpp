#include "MqttConAckVarHeader.h"

MqttConAckVarHeader::MqttConAckVarHeader() : var_header({0}),
                                             return_code(RESERVED_CONNECT_RETURN_CODE)

{
}

MqttConAckVarHeader::MqttConAckVarHeader(uint8_t v[2]) : MqttConAckVarHeader()
{
    memcpy(this->var_header, v, 2);
    this->return_code = (connect_return_code_e)(v[1]);
}

MqttConAckVarHeader::~MqttConAckVarHeader()
{
}
