#ifndef COMMAND_H_
#define COMMAND_H_

#include "common.h"
#include "simcom_definitions.h"
#include "Simcom_Resp.h"
#include "SimcomUart.h"
#include <list>

using namespace std;

enum value_type_e
{
    NUMBER,
    STRING
};

class Value
{
private:
    string value_string;
    int value_int;
    value_type_e type;

public:
    /**
     * Constructor
     * @author EKPV
     */
    Value(const char c);

    /**
     * Constructor
     * @author EKPV
     */
    Value(string value);

    /**
     * Constructor
     * @author EKPV
     */
    Value(int value);

    /**
     * Destructor
     * @author EKPV
     */
    ~Value();

    /**
     * Get value string
     * @author EKPV
     * @return string
     */
    string get_value_string();

    /**
     * Get value int
     * @author EKPV
     * @return int
     */
    int get_value_int();

    /**
     * Get value type
     * @author EKPV
     * @return value_type_e
     */
    value_type_e get_type();
};

class Command
{
private:
    const char *cmd;
    CMD_action_enum action;
    list<Value> values;
    char msg_send[MAX_NUM_CHAR_SEND_BUFF];
    uint16_t size;

public:
    /**
     * Constructor
     * @author EKPV
     */
    Command();

    /**
     * Constructor
     * @author EKPV
     * @param cmd : const char *
     * @param action : CMD_action_enum
     */
    Command(const char *cmd, CMD_action_enum action);

    /**
     * Destructor
     * @author EKPV
     */
    ~Command();

    /**
     * Add value to command list of values to send to GSM module
     * @author EKPV
     * @param value : Value
     * @return void
     */
    void add_value(Value value);

    /**
     * Build command to send to GSM module
     * @author EKPV
     * @param msg_send : char *
     * @param size : uint16_t *
     * @return void
     */
    void build(char *msg_send, uint16_t *size);
};

#endif /* COMMAND_H_ */