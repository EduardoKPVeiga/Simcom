#ifndef MQTTSUBTOPIC_H
#define MQTTSUBTOPIC_H

#include <cstdint>
#include "mtw_str.h"
#include "MqttDefinitions.h"

class MqttSubTopic
{
private:
public:
    uint16_t length;
    string name;
    qos_e qos_level;

    MqttSubTopic(string n, qos_e q);
    ~MqttSubTopic();
};

#endif // MQTTSUBTOPIC_H