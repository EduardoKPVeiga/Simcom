#include "MqttPacketFixedHeader.h"

MqttPacketFixedHeader::MqttPacketFixedHeader() : fix_header({0}),
                                                 type(RESERVED_MSG_TYPE),
                                                 dup_flag(0),
                                                 qos(RESERVED_QOS),
                                                 retain(0),
                                                 remaining_length(0)
{
}

MqttPacketFixedHeader::MqttPacketFixedHeader(uint8_t data[2]) : MqttPacketFixedHeader()
{
    memcpy(this->fix_header, data, 2);
    this->decode();
}

MqttPacketFixedHeader::MqttPacketFixedHeader(msg_type_e t, uint8_t d, qos_e q, uint8_t r, uint16_t r_len) : fix_header({0}),
                                                                                                            type(t),
                                                                                                            dup_flag(d),
                                                                                                            qos(q),
                                                                                                            retain(r),
                                                                                                            remaining_length(r_len)
{
    this->encode();
}

MqttPacketFixedHeader::~MqttPacketFixedHeader()
{
}

void MqttPacketFixedHeader::decode()
{
    this->type = (msg_type_e)(this->fix_header[0] >> 4);
    this->dup_flag = (this->fix_header[0] >> 3) & 0x01;
    this->qos = (qos_e)((this->fix_header[0] >> 1) & 0x03);
    this->retain = this->fix_header[0] & 0x01;
    this->remaining_length = this->fix_header[1];
}

void MqttPacketFixedHeader::encode()
{
    this->fix_header[0] = (uint8_t)type << 4;
    this->fix_header[0] |= dup_flag << 3;
    this->fix_header[0] |= (uint8_t)qos << 1;
    this->fix_header[0] |= retain;
    this->fix_header[1] = remaining_length;
}
