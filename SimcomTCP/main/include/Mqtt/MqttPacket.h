#ifndef MQTTPACK_H
#define MQTTPACK_H

#include <cstdint>
#include "MqttPacketFixedHeader.h"

#define MAX_DATA_LENGTH 1024

class MqttPacket
{
protected:
    uint8_t data[MAX_DATA_LENGTH];
    uint16_t data_size;
    MqttPacketFixedHeader fixed_header;

public:
    MqttPacket();
    MqttPacket(uint8_t *d, uint16_t d_size);
    MqttPacket(msg_type_e t, uint8_t d, qos_e q, uint8_t r, uint16_t r_len);
    ~MqttPacket();

    uint8_t *buffer();
    uint16_t buffer_size();
};

#endif // MQTTPACK_H