#include "SimcomResp.h"

SimcomResp::SimcomResp() : from(""),
                           msg(""),
                           size(0),
                           not_used(0) {}

SimcomResp::SimcomResp(string data, size_t _size) : from(""),
                                                    msg(""),
                                                    size(0),
                                                    not_used(0)
{
    extract_msg(data, _size);
    extract_from(data, _size);
}

SimcomResp::SimcomResp(string _msg, string _from, uint16_t _size) : from(_from),
                                                                    msg(_msg),
                                                                    size(_size),
                                                                    not_used(0) {}

SimcomResp::~SimcomResp() {}

void SimcomResp::extract_msg(string data, uint16_t _size)
{
    int i = 0;
    int begin_cmd_index = -1;

    begin_cmd_index = data.find(BEGIN_CMD);
    if (begin_cmd_index >= 0)
        i = data.find('\n', begin_cmd_index + SIZE(BEGIN_CMD)) + 1;
    else
    {
        i = data.find(':') + 1;
        if (i < 1)
            i = 0;
    }

    for (; i < size; i++)
    {
        if (data[i] != '\r' && data[i] != '\0')
            msg += data[i];
    }
    msg.erase(remove(msg.begin(), msg.end(), '\r'), msg.end());
    msg.erase(remove(msg.begin(), msg.end(), '\n'), msg.end());
}

void SimcomResp::extract_from(string data, uint16_t _size)
{
    int index = 0;
    int begin_cmd_index = -1;

    begin_cmd_index = data.find(BEGIN_CMD);
    if (begin_cmd_index >= 0)
        index = data.find('\n', begin_cmd_index + SIZE(BEGIN_CMD)) + 1;
    else
    {
        index = data.find(':') + 1;
        if (index < 1)
            index = 0;
    }

    for (int i = 0; i < index; i++)
    {
        if (data[i] != '\r' && data[i] != '\0')
            from += data[i];
    }
    from.erase(remove(from.begin(), from.end(), '\r'), from.end());
    from.erase(remove(from.begin(), from.end(), '\n'), from.end());
}

bool SimcomResp::valid(Command cmd)
{
    // Validar resposta
    return msg.find(RESP_OK);
}