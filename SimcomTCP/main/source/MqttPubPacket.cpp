#include "MqttPubPacket.h"

MqttPubPacket::MqttPubPacket(string t, string p) : MqttPacket(t, p)
{
}

MqttPubPacket::~MqttPubPacket()
{
}

void MqttPubPacket::create_packet()
{
    int remaining_length = 2 + topic_size + payload_size; // Tamanho correto do pacote
    int fixed_header_length = 4;

    if (remaining_length > 127)
    {
        return; // Erro: tamanho excede o limite de um único byte no campo Remaining Length
    }

    // buffer[0] = 0x30;             // MQTT PUBLISH header (QoS 0, no retain)
    buffer[0] = 0x32;             // MQTT PUBLISH header (QoS 1)
    buffer[1] = remaining_length; // Remaining length

    buffer[2] = (topic_size >> 8) & 0xFF; // Topic Length MSB
    buffer[3] = topic_size & 0xFF;        // Topic Length LSB

    memcpy(&(buffer[4]), topic, topic_size); // Copia o nome do tópico

    int msg_id = 10;
    buffer[fixed_header_length + topic_size] = (msg_id >> 8) & 0xFF; // Topic Length MSB
    buffer[fixed_header_length + topic_size + 1] = msg_id & 0xFF;    // Topic Length LSB

    memcpy(&(buffer[fixed_header_length + topic_size + 2]), payload, payload_size); // Copia o payload

    buffer_size = fixed_header_length + topic_size + payload_size; // Atualiza o tamanho total do buffer
}