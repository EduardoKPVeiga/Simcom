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
    uint16_t index_end = mtw_str::StrContainsChar(&(msg[index_begin]), (char)0x0D, size) + index_begin;
    this->msg = string(&(msg[index_begin]), index_end - index_begin);
    this->size_m = index_end - index_begin;

    uint16_t id_begin = mtw_str::StrContainsChar(msg, ',', size) + 1;
    uint16_t id_end = mtw_str::StrContainsChar(&(msg[id_begin + 1]), ',', size - id_begin - 1) + id_begin + 1;
    this->id = stoi(string(&(msg[id_begin]), id_end - id_begin));

    uint16_t length_begin = id_end + 1;
    uint16_t length_end = index_end;
    this->length = stoi(string(&(msg[length_begin]), length_end - length_begin));

    uint16_t data_begin = index_end + 2;
    for (int i = 0; i < this->length; i++)
        this->data[i] = msg[data_begin + i];
}

Caurc::~Caurc()
{
}