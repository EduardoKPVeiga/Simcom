#include "Simcom.h"

#define TAG "Simcom"

Simcom::Simcom() : simcomUart(SimcomUart()), serial_num("")
{
    simcomUart.open();
}

Simcom::Simcom(string sn) : Simcom()
{
    serial_num = sn;
}

Simcom::~Simcom()
{
}

bool Simcom::power(bool pwr)
{
    bool v = true;
    simcomUart.close();
    v = pwr ? pwrkey_power_on() : pwrkey_power_off();
    simcomUart.open();
    return v;
}

SimcomResp Simcom::send(Command cmd)
{
    cmd.build();
    simcomUart.send(cmd);
    SimcomResp resp = simcomUart.get_resp(cmd);
    if (mtw_str::StrContainsSubstr((char *)(cmd.cmd), CARECV, SIZE(CARECV), SIZE(CARECV)) >= 0)
    {
        if (!resp.msg.contains(RESP_ERROR))
        {
            // +CARECV: <size>,<data>
            uint16_t index = resp.msg.find(':') + 2;
            uint16_t comma_pos = resp.msg.find(',');
            string s = resp.msg.substr(index, comma_pos - index);
            int size = stoi(s);
            char *r = new char[size];
            index = comma_pos + 1;
            for (int i = 0; i < size; i++)
            {
                r[i] = resp.msg[index + i];
            }
            cout << "Size: " << size << endl
                 << "HEX: " << mtw_str::to_hex_string(r, size) << endl;
            MqttMsgAck ack = MqttMsgAck(r, size);
            ack.decode();
        }
    }
    return resp;
}

SimcomResp Simcom::send(Casend casend_cmd)
{
    casend_cmd.build();
    simcomUart.send(casend_cmd);
    return simcomUart.get_resp(casend_cmd);
}

SimcomResp Simcom::get_resp(Command cmd)
{
    return simcomUart.get_resp(cmd);
}

bool Simcom::mqtt_connect(string s_ip, int port)
{
    SimcomResp resp = SimcomResp();
    Command cmd = Command();

    cmd = Command(CAOPEN, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)0));
    cmd.add_value(Value("TCP"));
    cmd.add_value(Value(s_ip));
    cmd.add_value(Value(port));
    cmd.add_value(Value((int)1));
    resp = this->send(cmd);
    if (!resp.valid(cmd))
        return false;

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    MqttConPacket c_packet = MqttConPacket(this->serial_num);
    c_packet.create_packet();
    Casend casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, c_packet.buffer, c_packet.buffer_size);
    cmd = (Command)casend_cmd;
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)c_packet.buffer_size));
    resp = this->send(casend_cmd);
    if (!resp.valid(cmd))
        return false;

    MqttSubPacket s_packet = MqttSubPacket("S1mC0M5/2", 1564);
    s_packet.create_packet();
    casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, s_packet.buffer, s_packet.buffer_size);
    cmd = (Command)casend_cmd;
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)s_packet.buffer_size));
    resp = this->send(casend_cmd);
    if (!resp.valid(cmd))
        return false;

    return true;
}

bool Simcom::start_network()
{
    SimcomResp resp = SimcomResp();
    Command cmd = Command();

    cmd = Command(CSQ, CMD_action_enum::EXE);
    resp = this->send(cmd);
    if (!resp.valid(cmd))
        return false;

    cmd = Command(CGNAPN, CMD_action_enum::EXE);
    resp = this->send(cmd);
    if (!resp.valid(cmd))
        return false;

    cmd = Command(CNCFG, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)1));
    cmd.add_value(Value("ctnb"));
    resp = this->send(cmd);
    if (!resp.valid(cmd))
        return false;

    cmd = Command(CNACT, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    cmd.add_value(Value((int)1));
    resp = this->send(cmd);
    if (!resp.valid(cmd))
    {
        cmd = Command(CNACT, CMD_action_enum::READ);
        resp = this->send(cmd);
        int index = resp.msg.find(':') + 5;
        if (resp.msg[index] == 0)
            return false;
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    resp = this->get_resp(Command("+APP PDP", CMD_action_enum::EXE));
    if (resp.msg.contains("DEACTIVE"))
    {
        ESP_LOGE(TAG, "Network not activated.");
        return false;
    }
    return true;
}

bool Simcom::close_connection()
{
    Command cmd = Command(CACLOSE, CMD_action_enum::WRITE);
    cmd.add_value(Value((int)0));
    this->send(cmd);
    return true;
}

bool Simcom::mqtt_send_msg(string topic, string msg)
{
    MqttPubPacket p_packet = MqttPubPacket(topic, msg);
    p_packet.create_packet();
    Casend casend_cmd = Casend(CASEND, CMD_action_enum::WRITE, p_packet.buffer, p_packet.buffer_size);
    casend_cmd.add_value(Value((int)0));
    casend_cmd.add_value(Value((int)p_packet.buffer_size));
    SimcomResp resp = this->send(casend_cmd);
    return resp.valid(casend_cmd);
}