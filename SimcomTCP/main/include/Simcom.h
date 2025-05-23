#ifndef SIMCOM_H
#define SIMCOM_H

#include "SimcomUart.h"
#include "simcom_definitions.h"
#include "MqttMsgAck.h"
#include "MqttConPacket.h"
#include "MqttSubPacket.h"
#include "MqttPubPacket.h"
#include "MqttPingPacket.h"
#include "MqttDiscPacket.h"

#define SN_SIZE 7

class Simcom
{
private:
    SimcomUart simcomUart;
    string serial_num;

    SimcomResp send(Command cmd);
    Caurc send(Casend cmd);
    SimcomResp get_resp(Command cmd);

public:
    Simcom();
    Simcom(string sn);
    ~Simcom();

    bool power(bool pwr);

    bool mqtt_connect(string s_ip, int port);
    bool mqtt_disconnect();
    bool mqtt_publish(string topic, string msg);
    bool mqtt_subscribe(string topic, uint16_t id);
    bool mqtt_connected();

    bool network_connect();
    bool network_disconnect();
};

#endif // SIMCOM_H