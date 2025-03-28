#include "MqttSubTopic.h"

MqttSubTopic::MqttSubTopic(string n, qos_e q) : length(n.length()),
                                                name(n),
                                                qos_level(q)
{
}

MqttSubTopic::~MqttSubTopic()
{
}