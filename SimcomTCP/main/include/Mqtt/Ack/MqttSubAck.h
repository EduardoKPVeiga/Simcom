#ifndef MQTTSUBACK_H
#define MQTTSUBACK_H

#include "MqttPacket.h"
#include "MqttSubAckVarHeader.h"
#include <list>

using namespace std;

class MqttSubAck : public MqttPacket
{
private:
    MqttSubAckVarHeader var_header;

public:
    list<qos_e> qos_level_list;

    MqttSubAck();
    MqttSubAck(uint8_t *d, uint16_t d_size);
    ~MqttSubAck();
};

#endif // MQTTSUBACK_H