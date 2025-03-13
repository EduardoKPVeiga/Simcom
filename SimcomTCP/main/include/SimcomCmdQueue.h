#ifndef SIMCOM_CMD_QUEUE_H
#define SIMCOM_CMD_QUEUE_H

#include <queue>
#include "Command.h"
#include "Casend.h"

enum cmd_queue_type_e
{
    EMPTY = 0,
    PDN_AUTO_ACT = 1
};

class SimcomCmdQueue
{
private:
    cmd_queue_type_e type;
    queue<Command> cmd_queue;
    queue<Casend> casend_queue;

public:
    SimcomCmdQueue(cmd_queue_type_e t);
    ~SimcomCmdQueue();

    void clear();
    int size();
    void enqueue(Command cmd);
    Command dequeue();
    void enqueue_casend(Casend cmd);
    Casend dequeue_casend();
    bool is_empty();
};

#endif // SIMCOM_CMD_QUEUE_H