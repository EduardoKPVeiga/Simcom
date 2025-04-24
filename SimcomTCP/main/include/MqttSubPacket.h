#ifndef MQTTSUBPACKET_H
#define MQTTSUBPACKET_H

#include "MqttPacket.h"

class MqttSubPacket : public MqttPacket
{
private:
public:
    MqttSubPacket(string t, uint16_t id);
    ~MqttSubPacket();

    void create_packet() override;
};

#endif // MQTTSUBPACKET_H