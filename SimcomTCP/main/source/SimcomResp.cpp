#include "SimcomResp.h"

SimcomResp::SimcomResp() : from(""),
                           msg(""),
                           size_f(0),
                           size_m(0),
                           not_used(0) {}

SimcomResp::SimcomResp(string data, size_t size) : from(""),
                                                   msg(""),
                                                   size_f(0),
                                                   size_m(0),
                                                   not_used(0)
{
    extract_msg(data, size);
    extract_from(data, size);
}

SimcomResp::~SimcomResp() {}

void SimcomResp::extract_msg(string data, size_t size)
{
    int i = 0;
    if (data.contains('\n') && data.find('\n') + 1 <= size)
    {
        i = data.find('\n') + 1;
    }
    else
    {
        if (data.contains(':'))
            i = data.find(':') + 1;
        else if (data.contains(' '))
            i = data.find(' ') + 1;
        else
            i = size;
    }

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
    if (data.contains('\n'))
        index = data.find('\n');
    else if (data.contains(':'))
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