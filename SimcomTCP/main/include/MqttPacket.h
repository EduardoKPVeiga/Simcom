#ifndef MQTTPACKET_H
#define MQTTPACKET_H

#include <cstdlib>
#include "simcom_definitions.h"
#include "mtw_str.h"

class MqttPacket
{
public:
    uint16_t buffer_size;
    uint16_t topic_size;
    uint16_t payload_size;

    uint16_t packet_id;

    char buffer[MAX_NUM_CHAR_SEND_BUFF];
    char topic[MAX_NUM_CHAR_SEND_BUFF];
    char payload[MAX_NUM_CHAR_SEND_BUFF];

    MqttPacket();
    MqttPacket(string t);
    MqttPacket(string t, string p);
    ~MqttPacket();

    void set_topic(char *t);
    void set_payload(char *p);

    virtual void create_packet() = 0;
};

#endif // MQTTPACKET_H