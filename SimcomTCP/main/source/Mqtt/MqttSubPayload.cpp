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