#ifndef MQTTSUBACKVARHEADER_H
#define MQTTSUBACKVARHEADER_H

#include <cstdint>
#include <cstring>

class MqttSubAckVarHeader
{
private:
    uint8_t variable_header[2];

public:
    uint16_t msg_id;

    MqttSubAckVarHeader();
    MqttSubAckVarHeader(uint8_t v[2]);
    ~MqttSubAckVarHeader();
};

#endif // MQTTSUBACKVARHEADER_H
