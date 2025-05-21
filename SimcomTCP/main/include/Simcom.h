#ifndef SIMCOM_H
#define SIMCOM_H

#include "SimcomUart.h"
#include "simcom_definitions.h"
#include "MqttMsgAck.h"
#include "MqttConPacket.h"
#include "MqttSubPacket.h"
#include "MqttPubPacket.h"

#define SN_SIZE 7

class Simcom
{
private:
    SimcomUart simcomUart;
    string serial_num;

    SimcomResp send(Command cmd);
    SimcomResp send(Casend cmd);
    SimcomResp get_resp(Command cmd);

public:
    Simcom();
    Simcom(string sn);
    ~Simcom();

    bool power(bool pwr);

    bool mqtt_connect(string s_ip, int port);
    bool start_network();
    bool mqtt_send_msg(string topic, string msg);
    bool close_connection();
};

#endif // SIMCOM_H