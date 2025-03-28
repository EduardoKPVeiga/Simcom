#include "MqttPub.h"

MqttPub::MqttPub(uint16_t id) : MqttPacket(),
                                var_header(MqttPubVarHeader(0)),
                                data_pub({0}),
                                data_pub_size(0)
{
}

MqttPub::~MqttPub()
{
}

void MqttPub::set(string topic, uint8_t *d_p, uint16_t d_p_size)
{
    this->var_header.set_topic(topic);
    this->data_pub_size = d_p_size;
    memcpy(this->data_pub, d_p, d_p_size);
    uint16_t remaining_length = this->var_header.get_length() + d_p_size;
    this->fixed_header = MqttPacketFixedHeader(msg_type_e::PUBLISH, 0, qos_e::AT_LEAST_ONCE, 0, remaining_length);
}

void MqttPub::build()
{
    memcpy(this->data, this->fixed_header.fix_header, 2);
    this->data_size = 2;
    uint16_t v_size = this->var_header.size();
    memcpy(&(this->data[this->data_size]), this->var_header.variable_header, v_size);
    this->data_size += v_size;
    memcpy(&(this->data[this->data_size]), this->data_pub, this->data_pub_size);
    this->data_size += this->data_pub_size;
}