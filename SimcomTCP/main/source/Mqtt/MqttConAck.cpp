#include "MqttConAck.h"

MqttConAck::MqttConAck() : MqttPacket(),
                           variable_header(MqttConAckVarHeader())
{
}

MqttConAck::MqttConAck(uint8_t *d, uint16_t d_size) : MqttPacket(d, d_size),
                                                      variable_header(MqttConAckVarHeader())
{
    if (this->data_size >= 4)
        this->variable_header = MqttConAckVarHeader(&(this->data[2]));
}

MqttConAck::~MqttConAck()
{
}
