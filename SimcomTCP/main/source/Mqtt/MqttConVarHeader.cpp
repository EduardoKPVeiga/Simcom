#include "MqttConVarHeader.h"

MqttConVarHeader::MqttConVarHeader() : length(6),
                                       v_name("MQIsdp"),
                                       protocol_v_num(3),
                                       variable_header({0}),
                                       user_name_flag(0),
                                       password_flag(0),
                                       will_retain(0),
                                       will_qos(qos_e::AT_MOST_ONCE),
                                       will_flag(0),
                                       clean_session(1),
                                       keep_alive_timer(60)

{
    this->variable_header[0] = this->length >> 8;
    this->variable_header[1] = this->length & 0x00FF;
    for (int i = 0; i < this->length; i++)
        this->variable_header[2 + i] = this->v_name[i];
    this->variable_header[8] = this->protocol_v_num;

    uint8_t connect_flags = 0;
    connect_flags = this->user_name_flag << 7;
    connect_flags |= this->password_flag << 6;
    connect_flags |= this->will_retain << 5;
    connect_flags |= this->will_qos << 3;
    connect_flags |= this->will_flag << 2;
    connect_flags |= this->clean_session << 1;

    this->variable_header[9] = connect_flags;
    this->variable_header[10] = this->keep_alive_timer >> 8;
    this->variable_header[11] = this->keep_alive_timer & 0x00FF;
}

MqttConVarHeader::~MqttConVarHeader()
{
}

void MqttConVarHeader::set_user_password_flags(bool u)
{
    this->user_name_flag = u;
    this->password_flag = u;

    uint8_t connect_flags = this->variable_header[9] & 0x3F;

    connect_flags = u << 7;
    connect_flags |= u << 6;

    this->variable_header[9] = connect_flags;
}

void MqttConVarHeader::set_keep_alive_timer(uint16_t k)
{
    this->keep_alive_timer = k;

    this->variable_header[10] = k >> 8;
    this->variable_header[11] = k & 0x00FF;
}

void MqttConVarHeader::set_will_msg(bool w, qos_e q, bool r)
{
    this->will_flag = w;
    this->will_qos = q;
    this->will_retain = r;

    uint8_t connect_flags = 0;
    connect_flags |= r << 5;
    connect_flags |= q << 3;
    connect_flags |= w << 3;

    uint8_t connect_flags_h = this->variable_header[9] & 0xC0;
    uint8_t connect_flags_l = this->variable_header[9] & 0x03;

    connect_flags |= connect_flags_h;
    connect_flags |= connect_flags_l;

    this->variable_header[9] = connect_flags;
}