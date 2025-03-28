#ifndef MQTTPACKETFIXEDHEADER_H
#define MQTTPACKETFIXEDHEADER_H

#include <cstdint>
#include <cstring>
#include "MqttDefinitions.h"

class MqttPacketFixedHeader
{
private:
    void decode();
    void encode();

public:
    uint8_t fix_header[2];
    msg_type_e type;
    uint8_t dup_flag;
    qos_e qos;
    uint8_t retain;
    uint8_t remaining_length;

    MqttPacketFixedHeader();
    MqttPacketFixedHeader(uint8_t data[2]);
    MqttPacketFixedHeader(msg_type_e t, uint8_t d, qos_e q, uint8_t r, uint16_t r_len);
    ~MqttPacketFixedHeader();
};

#endif // MQTTPACKETFIXEDHEADER_H