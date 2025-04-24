#ifndef MQTTPUBPACKET_H
#define MQTTPUBPACKET_H

#include "MqttPacket.h"

class MqttPubPacket : public MqttPacket
{
public:
    MqttPubPacket(string t, string p);
    ~MqttPubPacket();

    void create_packet() override;
};

#endif // MQTTPUBPACKET_H