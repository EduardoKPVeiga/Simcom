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
    // simcomUart.close();
    v = pwr ? pwrkey_power_on() : pwrkey_power_off();
    // simcomUart.open();
    return v;
}

void Simcom::set_queue(SimcomCmdQueue queue)
{
    cmd_queue.clear();
    cmd_queue = queue;
    simcomUart.simcom_resp_list.clear();
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

            if (mtw_str::StrContainsSubstr((char *)(cmd.cmd), CARECV, SIZE(CARECV), SIZE(CARECV)) >= 0)
            {
                cout << "Received: " << resp.msg << endl;
                char r[4] = {0, 0, 0, 0};
                uint16_t index = resp.msg.find(',') + 1;

                for (int i = 0; i < 4; i++)
                {
                    r[i] = resp.msg[index + i];
                }
                cout << "HEX: " << mtw_str::to_hex_string(r, 4) << endl
                     << endl;
            }

            // Validar resposta
            if (!resp.valid(cmd))
            {
                return false;
            }
        }
    }
    return true;
}