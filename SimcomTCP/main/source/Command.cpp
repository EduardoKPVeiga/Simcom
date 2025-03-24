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
    for (int i = 0; i < SIZE(BEGIN_CMD); i++)
    {
        msg_send[i] = BEGIN_CMD[i];
        size++;
    }

    // Add command
    for (int i = 0; i < strlen(cmd); i++)
    {
        msg_send[size] = cmd[i];
        size++;
    }
    if (this->action == WRITE)
    {
        msg_send[size] = WRITE_CMD[0];
        size++;
    }
    else if (action == READ)
    {
        msg_send[size] = READ_CMD[0];
        size++;
    }
    else if (action == TEST)
    {
        msg_send[size] = TEST_CMD[0];
        size++;
        msg_send[size] = TEST_CMD[1];
        size++;
    }

    // Add values
    for (list<Value>::iterator it = this->values.begin(); it != this->values.end(); ++it)
    {
        if (it->get_type() == value_type_e::STRING)
        {
            // Add string
            const char *str = it->get_value_string().c_str();

            // Add quotation marks
            msg_send[size] = '"';
            size++;
            for (int i = 0; i < strlen(str); i++)
            {
                msg_send[size] = str[i];
                size++;
            }

            // Add quotation marks
            msg_send[size] = '"';
            size++;
        }
        else if (it->get_type() == value_type_e::NUMBER)
        {
            // Add number
            int num = it->get_value_int();
            const char *a = mtw_str::decimal_to_char_array(num);
            for (int i = 0; i < strlen(a); i++)
            {
                msg_send[size] = a[i];
                size++;
            }
        }
        msg_send[size] = VALUE_DELIMITER[0];
        size++;
    }
    if (this->action == WRITE)
        size--;

    // Add end command
    int aux = size;
    for (int i = 0; i < SIZE(END_CMD); i++)
    {
        msg_send[i + aux] = END_CMD[i];
        size++;
    }
}

void Command::add_value(Value value)
{
    this->values.push_back(value);
}