#ifndef MQTTPUBVARHEADER_H
#define MQTTPUBVARHEADER_H

#include <cstdint>
#include "mtw_str.h"
#include "MqttDefinitions.h"

class MqttPubVarHeader
{
private:
    uint16_t length;
    string pub_topic;
    uint16_t msg_id;

public:
    uint8_t variable_header[MAX_LENGTH_PUB_VAR_HEADER];

    MqttPubVarHeader(uint16_t id);
    ~MqttPubVarHeader();

    void set_topic(string topic);
    uint16_t get_length();
    uint16_t size();
};

#endif // MQTTPUBVARHEADER_H