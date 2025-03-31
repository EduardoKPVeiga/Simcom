#include "Command.h"

Value::Value(const char c)
{
    this->type = STRING;
    this->value_string = string(1, c);
    this->value_int = 0;
}
Value::Value(string value)
{
    this->type = STRING;
    this->value_string = value;
    this->value_int = 0;
}
Value::Value(int value)
{
    this->type = NUMBER;
    this->value_int = value;
    this->value_string = "";
}

Value::~Value()
{
    this->value_string = "";
    this->value_int = 0;
}

string Value::get_value_string()
{
    return this->value_string;
}

int Value::get_value_int()
{
    return this->value_int;
}

value_type_e Value::get_type()
{
    return this->type;
}

Command::Command()
{
    this->cmd = "";
    this->action = EXE;
}

Command::Command(const char *cmd, CMD_action_enum action)
{
    this->cmd = cmd;
    this->action = action;
}

Command::~Command()
{
    this->values.clear();
}

void Command::build()
{
    // Add begin command
    size = 0;
    memcpy(this->msg_send, BEGIN_CMD, SIZE(BEGIN_CMD));
    this->size += SIZE(BEGIN_CMD);

    // Add command
    memcpy(&(this->msg_send[this->size]), this->cmd, strlen(this->cmd));
    this->size += strlen(this->cmd);
    if (this->action == WRITE)
    {
        this->msg_send[this->size++] = WRITE_CMD[0];
    }
    else if (action == READ)
    {
        this->msg_send[this->size++] = READ_CMD[0];
    }
    else if (action == TEST)
    {
        this->msg_send[this->size++] = TEST_CMD[0];
        this->msg_send[this->size++] = TEST_CMD[1];
    }

    // Add values
    for (list<Value>::iterator it = this->values.begin(); it != this->values.end(); ++it)
    {
        if (it->get_type() == value_type_e::STRING)
        {
            // Add string
            const char *str = it->get_value_string().c_str();

            // Add quotation marks
            this->msg_send[this->size++] = '"';
            memcpy(&(this->msg_send[this->size]), str, strlen(str));
            this->size += strlen(str);

            // Add quotation marks
            this->msg_send[this->size++] = '"';
        }
        else if (it->get_type() == value_type_e::NUMBER)
        {
            // Add number
            int num = it->get_value_int();
            const char *a = mtw_str::decimal_to_char_array(num);

            memcpy(&(this->msg_send[this->size]), a, strlen(a));
            this->size += strlen(a);
        }
        this->msg_send[this->size++] = VALUE_DELIMITER[0];
    }
    if (this->action == WRITE)
        size--;

    // Add end command
    memcpy(&(this->msg_send[this->size]), END_CMD, SIZE(END_CMD));
    this->size += SIZE(END_CMD);
}

void Command::add_value(Value value)
{
    this->values.push_back(value);
}