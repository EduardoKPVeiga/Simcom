#include "MqttSubPayload.h"

MqttSubPayload::MqttSubPayload() : topic_list(list<MqttSubTopic>()),
                                   payload({0}),
                                   payload_size(0)
{
}

MqttSubPayload::~MqttSubPayload()
{
}

void MqttSubPayload::add(string t, qos_e q)
{
    this->topic_list.push_back(MqttSubTopic(t, q));
}

void MqttSubPayload::build()
{
    this->payload_size = 0;
    while (!topic_list.empty())
    {
        MqttSubTopic topic = topic_list.front();

        this->payload[this->payload_size++] = topic.length >> 8;
        this->payload[this->payload_size++] = topic.length & 0x00FF;

        memcpy(&(this->payload[this->payload_size]), topic.name.c_str(), topic.length);
        this->payload_size += topic.length;

        this->payload[this->payload_size++] = (uint8_t)topic.qos_level;
        topic_list.pop_front();
    }
}