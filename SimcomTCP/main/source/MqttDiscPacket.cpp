#include "MqttDiscPacket.h"

MqttDiscPacket::MqttDiscPacket() : MqttPacket()
{
}

MqttDiscPacket::~MqttDiscPacket()
{
}

void MqttDiscPacket::create_packet()
{
    this->buffer[0] = 0xE0; // MQTT DISCONNECT
    this->buffer[1] = 0x00;

    this->buffer_size = 2;
}