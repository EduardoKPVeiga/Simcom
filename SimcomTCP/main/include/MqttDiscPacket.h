#ifndef MQTTDISCPACKET_H
#define MQTTDISCPACKET_H

#include "MqttPacket.h"

class MqttDiscPacket : public MqttPacket
{
private:
public:
    MqttDiscPacket();
    ~MqttDiscPacket();

    void create_packet() override;
};

#endif