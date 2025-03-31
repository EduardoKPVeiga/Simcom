#include "MqttPubVarHeader.h"

MqttPubVarHeader::MqttPubVarHeader(uint16_t id) : length(0),
                                                  pub_topic(""),
                                                  msg_id(id),
                                                  variable_header({0})
{
}

MqttPubVarHeader::~MqttPubVarHeader()
{
}

void MqttPubVarHeader::set_topic(string topic)
{
    this->pub_topic = topic;
    this->length = topic.length();
}

uint16_t MqttPubVarHeader::get_length()
{
    return this->length;
}

uint16_t MqttPubVarHeader::size()
{
    return this->length + 4;
}