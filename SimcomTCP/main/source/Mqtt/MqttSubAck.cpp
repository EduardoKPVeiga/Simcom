#include "MqttSubAck.h"

MqttSubAck::MqttSubAck() : MqttPacket(),
                           var_header(MqttSubAckVarHeader()),
                           qos_level_list(list<qos_e>())
{
}

MqttSubAck::MqttSubAck(uint8_t *d, uint16_t d_size) : MqttPacket(d, d_size),
                                                      var_header(MqttSubAckVarHeader()),
                                                      qos_level_list(list<qos_e>())
{
    if (d_size >= 4)
    {
        this->var_header = MqttSubAckVarHeader(&(d[2]));
        for (int i = 4; i < d_size; i++)
            qos_level_list.push_back((qos_e)d[i]);
    }
}

MqttSubAck::~MqttSubAck()
{
}