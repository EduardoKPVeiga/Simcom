#include "MqttSub.h"

MqttSub::MqttSub(uint16_t id) : var_header(MqttSubVarHeader(id)),
                                payload(MqttSubPayload())
{
}

MqttSub::~MqttSub()
{
}

void MqttSub::add_topic(string t, qos_e q)
{
    this->payload.add(t, q);
}

void MqttSub::build()
{
    memcpy(this->data, this->fixed_header.fix_header, 2);
    this->data_size = 2;
    memcpy(&(this->data[this->data_size]), this->var_header.variable_header, 2);
    this->data_size += 2;
    memcpy(&(this->data[this->data_size]), this->payload.payload, this->payload.payload_size);
    this->data_size += this->payload.payload_size;
}