#ifndef MQTTSUBPAYLOAD_H
#define MQTTSUBPAYLOAD_H

#include "MqttSubTopic.h"
#include <list>
#include "MqttDefinitions.h"

class MqttSubPayload
{
private:
    list<MqttSubTopic> topic_list;

public:
    uint8_t payload[MAX_SUB_PAYLOAD_SIZE];
    uint16_t payload_size;

    MqttSubPayload();
    ~MqttSubPayload();

    void add(string t, qos_e q);
    void build();
};

#endif // MQTTSUBPAYLOAD_H