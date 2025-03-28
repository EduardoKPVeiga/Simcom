#ifndef MQTTCON_H
#define MQTTCON_H

#include "MqttPacket.h"
#include "MqttConVarHeader.h"
#include "mtw_str.h"

class MqttCon : public MqttPacket
{
private:
    MqttConVarHeader var_header;

public:
    string client_id;
    string will_topic;
    string will_msg;
    string user_name;
    string password;

    MqttCon();
    MqttCon(string c_id);
    ~MqttCon();

    void set_will_msg(string w_topic, string w_msg, qos_e q = qos_e::AT_MOST_ONCE);
    void set_will_msg(bool w);
    void set_user_login(string name, string pass);

    void build();
};

#endif // MQTTCON_H