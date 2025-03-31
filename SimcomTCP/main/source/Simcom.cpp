#include "Simcom.h"

Simcom::Simcom() : simcomUart(SimcomUart()), cmd_queue(SimcomCmdQueue(cmd_queue_type_e::EMPTY))
{
    simcomUart.open();
}

Simcom::Simcom(unsigned char sn[SN_SIZE]) : Simcom()
{
    memcpy(serial_num, sn, SN_SIZE);
    simcomUart.open();
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

void Simcom::set_queue(SimcomCmdQueue queue)
{
    cmd_queue.clear();
    cmd_queue = queue;
    simcomUart.simcom_resp_list.clear();
}

SimcomResp Simcom::send(Command cmd, uint16_t d)
{
    cmd.build();
    simcomUart.send(cmd);
    vTaskDelay(d / portTICK_PERIOD_MS);
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
            // MqttMsgAck ack = MqttMsgAck(r, size);
            // ack.decode();
        }
    }
    return resp;
}

SimcomResp Simcom::send(Casend casend_cmd, uint16_t d)
{
    casend_cmd.build();
    simcomUart.send(casend_cmd);
    vTaskDelay(d / portTICK_PERIOD_MS);
    return simcomUart.get_resp(casend_cmd);
}

SimcomResp Simcom::get_resp(Command cmd)
{
    return simcomUart.get_resp(cmd);
}