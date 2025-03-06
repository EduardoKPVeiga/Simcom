#ifndef CASEND_H
#define CASEND_H

#include "Command.h"

class Casend : public Command
{
private:
public:
    char data[MAX_NUM_CHAR_SEND_BUFF];
    uint16_t data_size;

    Casend();
    Casend(const char *cmd, CMD_action_enum action, char *data, uint16_t data_size);
    ~Casend();
};

#endif // CASEND_H