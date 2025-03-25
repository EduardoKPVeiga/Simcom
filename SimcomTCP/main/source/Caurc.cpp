#include "Caurc.h"

#define TAG "Caurc"

Caurc::Caurc() : SimcomResp(),
                 id(0),
                 length(0),
                 data({0})
{
}

Caurc::Caurc(char *msg, uint16_t size) : Caurc()
{
    // +CAURC: "recv",<id>,<length><CR><LF><data>
    this->from = CAURC;
    this->size_f = SIZE(CAURC);
    uint16_t index_begin = mtw_str::StrContainsChar(msg, '+', size) + SIZE(CAURC) + 1;
    uint16_t index_end = mtw_str::StrContainsChar(msg, '\0', size);
    this->msg = string(&(msg[index_begin]), index_end - index_begin);
    this->size_m = index_end - index_begin;

    uint16_t id_begin = mtw_str::StrContainsChar(msg, ',', size) + 1;
    uint16_t id_end = mtw_str::StrContainsChar(&(msg[id_begin + 1]), ',', size) + id_begin + 1;
    string id_s = string(&(msg[id_begin]), id_end - id_begin);
    this->id = stoi(id_s);

    uint16_t data_begin = index_end + 1;
    this->length = size - data_begin; // <CR><LF><data>
    for (int i = 0; i < this->length; i++)
    {
        this->data[i] = msg[data_begin + i];
    }
}

Caurc::~Caurc()
{
}