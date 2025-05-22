#ifndef MQTTPINGPACKET_H
#define MQTTPINGPACKET_H

#include "MqttPacket.h"

class MqttPingPacket : public MqttPacket
{
private:
public:
    MqttPingPacket();
    ~MqttPingPacket();

    void create_packet() override;
};

#endif