#ifndef MQTTPUB_H
#define MQTTPUB_H

#include "MqttPacket.h"
#include "mtw_str.h"
#include "MqttPubVarHeader.h"

class MqttPub : public MqttPacket
{
private:
    MqttPubVarHeader var_header;

public:
    uint8_t data_pub[MAX_SIZE_PUB_MSG];
    uint16_t data_pub_size;

    MqttPub(uint16_t id);
    ~MqttPub();

    void set(string topic, uint8_t *d_p, uint16_t d_p_size);
    void build();
};

#endif