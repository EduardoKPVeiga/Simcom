#include "MqttMsgAck.h"

#include "esp_log.h"
#include "esp_mac.h"

#define TAG "MqttMsgAck"

MqttMsgAck::MqttMsgAck() : type(msg_type_e::RESERVED_MSG_TYPE),
                           dup_flag(0),
                           qos(qos_e::RESERVED_QOS),
                           retain(0),
                           remaining_length(0),
                           connack_code(connect_return_code_e::RESERVED_CONNECT_RETURN_CODE)
{
}

MqttMsgAck::MqttMsgAck(char *data, uint16_t size) : MqttMsgAck()
{
    uint8_t byte = 0;

    byte = data[0];
    type = (msg_type_e)(byte >> 4);
    dup_flag = (byte >> 3) & 0x01;
    qos = (qos_e)((byte >> 1) & 0x03);
    retain = byte & 0x01;

    remaining_length = 0;
    byte = data[1];
    remaining_length = byte & 0x7F;
    while (byte & 0x80)
    {
        byte = data[2];
        remaining_length = (remaining_length << 7) + (byte & 0x7F);
    }

    if (type == msg_type_e::CONNACK && size >= 4)
    {
        connack_code = (connect_return_code_e)data[3];
        this->decode();
    }
}

MqttMsgAck::~MqttMsgAck()
{
}

void MqttMsgAck::decode()
{
    if (type == msg_type_e::CONNACK)
    {
        switch (connack_code)
        {
        case CONNECTION_ACCEPTED:
            ESP_LOGI(TAG, "Connection accepted.");
            break;

        case UNACCEPTABLE_PROTOCOL_VERSION:
            ESP_LOGE(TAG, "Unacceptable protocol version.");
            break;

        case IDENTIFIER_REJECTED:
            ESP_LOGE(TAG, "Identifier rejected.");
            break;

        case SERVER_UNAVAILABLE:
            ESP_LOGE(TAG, "Server unavailable.");
            break;

        case BAD_USERNAME_OR_PASSWORD:
            ESP_LOGE(TAG, "Bad username or password.");
            break;

        case NOT_AUTHORIZED:
            ESP_LOGE(TAG, "Not authorized.");
            break;

        case RESERVED_CONNECT_RETURN_CODE:
            ESP_LOGE(TAG, "Reserved connect return code.");
            break;

        default:
            break;
        }
    }
}