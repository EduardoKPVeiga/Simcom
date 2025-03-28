#ifndef MQTTCONACK_H
#define MQTTCONACK_H

#include "MqttPacket.h"
#include "MqttConAckVarHeader.h"

class MqttConAck : public MqttPacket
{
private:
public:
    MqttConAckVarHeader variable_header;

    MqttConAck();
    MqttConAck(uint8_t *d, uint16_t d_size);
    ~MqttConAck();
};

#endif // MQTTCONACK_H