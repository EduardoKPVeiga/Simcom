#include "MqttPingPacket.h"

MqttPingPacket::MqttPingPacket() : MqttPacket()
{
}

MqttPingPacket::~MqttPingPacket()
{
}

void MqttPingPacket::create_packet()
{
    this->buffer[0] = 0xC0; // MQTT PINGREQ packet type
    this->buffer[1] = 0x00; // Remaining length
    this->buffer_size = 2;
}
