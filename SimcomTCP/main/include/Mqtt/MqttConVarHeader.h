#ifndef MQTTCONVARHEADER_H
#define MQTTCONVARHEADER_H

#include <cstdint>
#include "mtw_str.h"
#include "MqttPacketFixedHeader.h"

class MqttConVarHeader
{
private:
    uint16_t length;
    string v_name;
    uint8_t protocol_v_num;

public:
    uint8_t variable_header[12];
    bool user_name_flag;
    bool password_flag;
    bool will_retain;
    qos_e will_qos;
    bool will_flag;
    bool clean_session;
    uint16_t keep_alive_timer;

    MqttConVarHeader();
    ~MqttConVarHeader();

    void set_will_msg(bool w, qos_e q = qos_e::RESERVED_QOS, bool r = false);
    void set_user_password_flags(bool u);
    void set_keep_alive_timer(uint16_t k);
};

#endif // MQTTCONVARHEADER_H