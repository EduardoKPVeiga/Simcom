#ifndef CAURC_H
#define CAURC_H

#include "SimcomResp.h"
#include "mtw_str.h"
#include "esp_log.h"

class Caurc : public SimcomResp
{
private:
public:
    uint16_t id;
    uint16_t length;
    char data[MAX_NUM_CHAR_RECEIVE_BUFF];

    Caurc();
    Caurc(char *msg, uint16_t size);
    ~Caurc();
};

#endif // CAURC_H
