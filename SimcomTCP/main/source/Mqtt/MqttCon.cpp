#include "MqttCon.h"

MqttCon::MqttCon() : MqttPacket(msg_type_e::CONNECT, 0, qos_e::AT_MOST_ONCE, 0, 12),
                     var_header(MqttConVarHeader()),
                     client_id("SNTESTE"),
                     will_topic(""),
                     will_msg(""),
                     user_name(""),
                     password("")
{
}

MqttCon::MqttCon(string c_id) : MqttCon()
{
    this->client_id = c_id;
}

MqttCon::~MqttCon()
{
}

void MqttCon::set_will_msg(string w_topic, string w_msg, qos_e q)
{
    this->var_header.set_will_msg(true, q);
    this->will_topic = w_topic;
    this->will_msg = w_msg;
}

void MqttCon::set_will_msg(bool w)
{
    this->var_header.set_will_msg(w);
}

void MqttCon::set_user_login(string name, string pass)
{
    this->var_header.set_user_password_flags(true);
    this->user_name = name;
    this->password = pass;
}

void MqttCon::build()
{
    memcpy(this->data, this->fixed_header.fix_header, 2);
    this->data_size = 2;
    memcpy(&(this->data[this->data_size]), this->var_header.variable_header, MQTT_VAR_HEADER_SIZE);
    this->data_size += MQTT_VAR_HEADER_SIZE;
    uint16_t client_id_length = this->client_id.length();
    this->data[this->data_size++] = client_id_length >> 8;
    this->data[this->data_size++] = client_id_length & 0x00FF;
    const char id[client_id_length] = {0};
    memcpy(&(this->data[this->data_size]), (this->client_id).c_str(), client_id_length);
}