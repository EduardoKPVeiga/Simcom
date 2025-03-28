#ifndef MQTTSUBVARHEADER_H
#define MQTTSUBVARHEADER_H

#include <cstdint>

class MqttSubVarHeader
{
private:
    uint16_t msg_id;

public:
    uint8_t variable_header[2];

    MqttSubVarHeader(uint16_t id);
    ~MqttSubVarHeader();
};

#endif // MQTTSUBVARHEADER_H