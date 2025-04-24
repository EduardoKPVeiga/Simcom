#ifndef MQTTCONPACKET_H
#define MQTTCONPACKET_H

#include "MqttPacket.h"

class MqttConPacket : public MqttPacket
{
private:
public:
    string client_id;

    MqttConPacket(string c_id);
    ~MqttConPacket();

    void create_packet() override;
};

#endif
