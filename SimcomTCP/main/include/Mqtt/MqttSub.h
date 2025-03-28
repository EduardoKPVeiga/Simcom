#ifndef MQTTSUB_H
#define MQTTSUB_H

#include "MqttPacket.h"
#include "MqttSubVarHeader.h"
#include "MqttSubPayload.h"

class MqttSub : public MqttPacket
{
private:
    MqttSubVarHeader var_header;
    MqttSubPayload payload;

public:
    MqttSub(uint16_t id);
    ~MqttSub();

    void add_topic(string t, qos_e q);
    void build();
};

#endif // MQTTSUB_H