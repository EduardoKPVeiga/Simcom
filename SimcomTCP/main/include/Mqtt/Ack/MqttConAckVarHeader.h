#ifndef MQTTCONACKVARHEADER_H
#define MQTTCONACKVARHEADER_H

#include <cstdint>
#include <cstring>
#include "MqttDefinitions.h"

class MqttConAckVarHeader
{
private:
    uint8_t var_header[2];

public:
    connect_return_code_e return_code;

    MqttConAckVarHeader();
    MqttConAckVarHeader(uint8_t v[2]);
    ~MqttConAckVarHeader();
};

#endif // MQTTCONACKVARHEADER_H