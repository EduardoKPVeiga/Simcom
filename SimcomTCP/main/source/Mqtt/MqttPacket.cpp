#include "MqttPacket.h"

MqttPacket::MqttPacket() : data({0}),
                           data_size(0),
                           fixed_header(MqttPacketFixedHeader())
{
}

MqttPacket::MqttPacket(uint8_t *d, uint16_t d_size) : MqttPacket()
{
    this->data_size = d_size;
    if (d_size >= 2)
    {
        memcpy(this->data, d, d_size);
        this->fixed_header = MqttPacketFixedHeader(this->data);
    }
}

MqttPacket::MqttPacket(msg_type_e t, uint8_t d, qos_e q, uint8_t r, uint16_t r_len) : MqttPacket()
{
    this->fixed_header = MqttPacketFixedHeader(t, d, q, r, r_len);
}

MqttPacket::~MqttPacket()
{
}

uint8_t *MqttPacket::buffer()
{
    return this->data;
}

uint16_t MqttPacket::buffer_size()
{
    return this->data_size;
}
