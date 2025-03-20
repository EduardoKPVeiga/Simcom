#include "SimcomResp.h"

SimcomResp::SimcomResp() : from(""),
                           msg(""),
                           size_f(0),
                           size_m(0),
                           not_used(0) {}

SimcomResp::SimcomResp(list<string> data) : SimcomResp()
{
    string item;
    while (!data.empty())
    {
        item = data.front();
        if (item.contains("AT+"))
        {
            from = item;
            size_f = item.size();
        }
        else if (item.contains('+') && size_f == 0)
        {
            extract_from(item, item.size());
            extract_msg(item, item.size());
        }
        else
        {
            msg = item;
            size_m = item.size();
        }
        data.pop_front();
    }
}

SimcomResp::~SimcomResp() {}

void SimcomResp::extract_msg(string data, size_t size)
{
    int i = 0;
    if (data.contains(':'))
        i = data.find(':') + 1;
    else if (data.contains(' '))
        i = data.find(' ') + 1;
    else
        i = size;

    for (; i < size; i++)
    {
        if (data[i] != '\r' && data[i] != '\0')
            msg += data[i];
    }

    size_m = msg.size();
}

void SimcomResp::extract_from(string data, size_t size)
{
    int index = 0;
    if (data.contains(':'))
        index = data.find(':');
    else if (data.contains(' '))
        index = data.find(' ');
    else
        index = size;

    for (int i = 0; i < index; i++)
    {
        if (data[i] != '\r' && data[i] != '\0')
            from += data[i];
    }

    size_f = from.size();
}

bool SimcomResp::valid(Command cmd)
{
    return msg.contains(RESP_OK);
}