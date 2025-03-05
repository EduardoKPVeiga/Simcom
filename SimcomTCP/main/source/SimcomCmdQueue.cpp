#include "SimcomCmdQueue.h"

SimcomCmdQueue::SimcomCmdQueue(cmd_queue_type_e t) : type(t), cmd_queue(queue<Command>())
{
}

SimcomCmdQueue::~SimcomCmdQueue()
{
    while (!cmd_queue.empty())
        cmd_queue.pop();
}

void SimcomCmdQueue::enqueue(Command cmd)
{
    cmd_queue.push(cmd);
}

Command SimcomCmdQueue::dequeue()
{
    Command cmd = cmd_queue.front();
    cmd_queue.pop();
    return cmd;
}

bool SimcomCmdQueue::is_empty()
{
    return cmd_queue.empty();
}