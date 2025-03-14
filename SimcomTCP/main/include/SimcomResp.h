#ifndef GSM_RESP_H_
#define GSM_RESP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "mtw_str.h"
#include "simcom_definitions.h"
#include <algorithm> // for std::remove
#include <queue>
#include "Command.h"

#define MAX_RESP_NUM 10
#define MAX_NOT_USED_NUM 5

using namespace std;

class SimcomResp
{
private:
    void extract_msg(string data, size_t size);
    void extract_from(string data, size_t size);

public:
    string from;
    string msg;
    uint16_t size_f;
    uint16_t size_m;
    uint8_t not_used;

    /**
     * Constructor
     * @author EKPV
     */
    SimcomResp();

    /**
     * Constructor
     * @author EKPV
     * @param data : list<string>
     */
    SimcomResp(list<string> data);

    /**
     * Destructor
     * @author EKPV
     */
    ~SimcomResp();

    /**
     * Verify if the response is valid
     * @author EKPV
     * @param cmd : Command
     * @return bool
     */
    bool valid(Command cmd);
};

#endif