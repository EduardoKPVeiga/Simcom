#include "MqttSubPacket.h"

MqttSubPacket::MqttSubPacket(string t, uint16_t id) : MqttPacket(t)
{
    packet_id = id;
}

MqttSubPacket::~MqttSubPacket()
{
}

void MqttSubPacket::create_packet()
{
    int remaining_length = 2 + 2 + topic_size + 1; // ID do pacote (2 bytes) + Tamanho do tópico (2 bytes) + Tópico + QoS (1 byte)

    if (remaining_length > 127)
    {
        return; // Erro: tamanho excede o limite de um único byte no campo Remaining Length
    }

    buffer[0] = 0x82;             // MQTT SUBSCRIBE header
    buffer[1] = remaining_length; // Remaining length

    buffer[2] = (packet_id >> 8) & 0xFF; // Packet ID MSB
    buffer[3] = packet_id & 0xFF;        // Packet ID LSB

    buffer[4] = (topic_size >> 8) & 0xFF;  // Topic Length MSB
    buffer[5] = topic_size & 0xFF;         // Topic Length LSB
    memcpy(&buffer[6], topic, topic_size); // Copia o nome do tópico

    buffer[6 + topic_size] = 0x00; // QoS 0 (Entrega no máximo uma vez)

    buffer_size = 7 + topic_size; // Atualiza o tamanho total do buffer
}