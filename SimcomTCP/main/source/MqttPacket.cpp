#include "MqttPacket.h"

MqttPacket::MqttPacket() : buffer_size(0),
                           topic_size(0),
                           payload_size(0),
                           packet_id(0)
{
}

MqttPacket::MqttPacket(string t) : MqttPacket()
{
    topic_size = t.length();

    if (topic_size > 23)
    {
        return; // Erro: tamanho do tópico ou payload excede o limite
    }

    memcpy(topic, t.c_str(), topic_size);
}

MqttPacket::MqttPacket(string t, string p) : MqttPacket()
{
    topic_size = t.length();
    payload_size = p.length();

    if (topic_size > 23 || payload_size > 23)
    {
        return; // Erro: tamanho do tópico ou payload excede o limite
    }

    memcpy(topic, t.c_str(), topic_size);
    memcpy(payload, p.c_str(), payload_size);
}

MqttPacket::~MqttPacket()
{
}