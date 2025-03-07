#include "Simcom.h"

Simcom::Simcom() : simcomUart(SimcomUart()), cmd_queue(SimcomCmdQueue(cmd_queue_type_e::EMPTY))
{
    simcomUart.open();
}

Simcom::Simcom(unsigned char sn[SN_SIZE]) : simcomUart(SimcomUart()), cmd_queue(SimcomCmdQueue(cmd_queue_type_e::EMPTY))
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
    cmd_queue = queue;
}

bool Simcom::send()
{
    Command cmd;
    Casend casend_cmd;
    uint16_t size = 0;
    char msg_send[MAX_NUM_CHAR_SEND_BUFF] = {0};

    while (!cmd_queue.is_empty())
    {
        cmd = cmd_queue.dequeue();
        if (mtw_str::StrContainsSubstr((char *)(cmd.cmd), CASEND, SIZE(CASEND), SIZE(CASEND)) >= 0)
        {
            cout << "found." << endl;
            casend_cmd = cmd_queue.dequeue_casend();
            casend_cmd.build(msg_send, &size);
            simcomUart.send(msg_send, (size_t)size);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            simcomUart.send(casend_cmd.data, casend_cmd.data_size);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        else
        {
            cmd.build(msg_send, &size);
            simcomUart.send(msg_send, (size_t)size);
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            SimcomResp resp = simcomUart.get_resp(cmd);
            // Validar resposta
            if (!resp.valid(cmd))
                return false;
        }
    }
    return true;
}