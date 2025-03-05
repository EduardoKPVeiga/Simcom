#ifndef GSM_RESP_H_
#define GSM_RESP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "mtw_str.h"
#include "simcom_definitions.h"
#include <algorithm> // for std::remove
#include <queue>

#define MAX_RESP_NUM 10
#define MAX_NOT_USED_NUM 5

using namespace std;

namespace gsm_resp
{
    /**
     * Initialize gsm_resp_semaphore
     * @return void
     */
    void init_gsm_resp_semaphore();

    class Gsm_Resp
    {
    public:
        string from;
        string msg;
        uint16_t size;
        uint8_t not_used;

        /**
         * Constructor
         * @author EKPV
         */
        Gsm_Resp();

        /**
         * Constructor
         * @author EKPV
         * @param _msg : string
         * @param _from : string
         * @param _size : uint16_t
         */
        Gsm_Resp(string _msg, string _from, uint16_t _size);

        /**
         * Destructor
         * @author EKPV
         */
        ~Gsm_Resp();
    };

    /**
     * resp_queue.empty()
     * @return bool
     */
    bool queue_empty();

    /**
     * resp_queue.size()
     * @return uint16_t
     */
    uint16_t queue_size();

    /**
     * Queue clear
     * @return void
     */
    void clear_queue();

    /**
     * Add an item to the resp_queue
     * @param item : Gsm_Resp
     */
    void enqueue(string msg, string from, uint16_t size);

    /**
     * Add an item to the resp_queue
     * @param item : Gsm_Resp
     * @return void
     */
    void enqueue(string msg, uint16_t size);

    /**
     * Remove the first item from queue
     * @return Gsm_Resp
     */
    Gsm_Resp dequeue();

    /**
     * Get an message from a specific command
     * @param cmd : string
     * @return Gsm_Resp
     */
    Gsm_Resp get_item(string cmd);

    /**
     * Get an message from a specific command
     * @param cmd : string
     * @param msg : string
     * @return Gsm_Resp
     */
    Gsm_Resp get_item(string cmd, string msg);

    /**
     * Extract from attribute
     * @param msg : string
     * @param size : uint16_t
     * @return string
     */
    string extract_from(string msg, uint16_t size);

    /**
     * Extract msg attribute
     * @param msg : char*
     * @param size : uint16_t
     * @return string
     */
    string extract_msg(string msg, uint16_t size);

    /**
     * Convert from char* to string
     * @param c_array : char*
     * @param size : uint16_t
     * @return string
     */
    string c_array_to_string(char *c_array, uint16_t size);
}

#endif