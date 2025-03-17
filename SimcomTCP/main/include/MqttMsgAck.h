#ifndef MQTTMSGACK_H
#define MQTTMSGACK_H

#include "mtw_str.h"
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_mac.h"

enum msg_type_e
{
    RESERVED_MSG_TYPE = 0,
    CONNECT = 1,
    CONNACK = 2,
    PUBLISH = 3,
    PUBACK = 4,
    PUBREC = 5,
    PUBREL = 6,
    PUBCOMP = 7,
    SUBSCRIBE = 8,
    SUBACK = 9,
    UNSUBSCRIBE = 10,
    UNSUBACK = 11,
    PINGREQ = 12,
    PINGRESP = 13,
    DISCONNECT = 14
};

enum qos_e
{
    AT_MOST_ONCE = 0,
    AT_LEAST_ONCE = 1,
    EXACTLY_ONCE = 2,
    RESERVED_QOS = 3
};

enum connect_return_code_e
{
    CONNECTION_ACCEPTED = 0,
    UNACCEPTABLE_PROTOCOL_VERSION = 1,
    IDENTIFIER_REJECTED = 2,
    SERVER_UNAVAILABLE = 3,
    BAD_USERNAME_OR_PASSWORD = 4,
    NOT_AUTHORIZED = 5,
    RESERVED_CONNECT_RETURN_CODE = 6
};

class MqttMsgAck
{
private:
    msg_type_e type;
    uint8_t dup_flag;
    qos_e qos;
    uint8_t retain;
    uint16_t remaining_length;
    connect_return_code_e connack_code;

public:
    MqttMsgAck();
    MqttMsgAck(char *data, uint16_t size);
    ~MqttMsgAck();

    void decode();
};

#endif // MQTTMSGACK_H