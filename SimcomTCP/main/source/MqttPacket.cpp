#include "MqttPacket.h"

MqttPacket::MqttPacket(const char *t, const char *p) : buffer_size(0),
                                                       topic_size(0),
                                                       payload_size(0),
                                                       packet_id(0)
{
    topic_size = strlen(t);
    payload_size = strlen(p);

    memcpy(topic, t, topic_size);
    memcpy(payload, p, payload_size);
}

MqttPacket::~MqttPacket()
{
}

int MqttPacket::create_publish_packet()
{
    int remaining_length = 2 + topic_size + payload_size; // Topic length (2 bytes) + topic + payload

    buffer[0] = 0x30;             // MQTT PUBLISH header (QoS 0, no retain)
    buffer[1] = remaining_length; // Remaining length field

    buffer[2] = (topic_size >> 8) & 0xFF; // Topic Length MSB
    buffer[3] = topic_size & 0xFF;        // Topic Length LSB

    memcpy(&buffer[4], topic, topic_size);                  // Copy topic name into the buffer
    memcpy(&buffer[4 + topic_size], payload, payload_size); // Copy payload into the buffer

    buffer_size = 4 + topic_size + payload_size; // Return total packet size
    return buffer_size;
}

int MqttPacket::create_subscribe_packet(uint16_t id)
{
    packet_id = id;
    int remaining_length = 2 + 2 + topic_size + 1; // Packet ID (2 bytes) + Topic Length (2 bytes) + Topic + QoS (1 byte)

    buffer[0] = 0x82; // MQTT SUBSCRIBE header
    buffer[1] = remaining_length;

    buffer[2] = (packet_id >> 8) & 0xFF; // Packet ID MSB
    buffer[3] = packet_id & 0xFF;        // Packet ID LSB

    buffer[4] = (topic_size >> 8) & 0xFF;  // Topic Length MSB
    buffer[5] = topic_size & 0xFF;         // Topic Length LSB
    memcpy(&buffer[6], topic, topic_size); // Topic

    buffer[6 + topic_size] = 0x00; // QoS 0 (At most once)

    buffer_size = 7 + topic_size; // Total packet size
    return buffer_size;
}

int MqttPacket::create_connect_packet(char *client_id, char *username, char *password)
{
    if (!client_id)
    {
        return -1; // Retorna erro se o client_id for NULL
    }

    int client_id_len = strlen(client_id);

    if (client_id_len > 23)
    {              // Limite padrão para muitos brokers
        return -2; // Código de erro para client_id muito grande
    }

    int remaining_length = 12 + (2 + client_id_len);

    if (remaining_length > 127)
    {
        return -3; // Tamanho excede o limite simples do protocolo MQTT
    }

    buffer[0] = 0x10; // MQTT CONNECT header
    buffer[1] = remaining_length;

    // Protocol Name "MQIsdp" (MQTT 3.1)
    buffer[2] = 0x00;
    buffer[3] = 0x06;
    buffer[4] = 'M';
    buffer[5] = 'Q';
    buffer[6] = 'I';
    buffer[7] = 's';
    buffer[8] = 'd';
    buffer[9] = 'p';

    buffer[10] = 0x03; // Protocol level (MQTT 3.1)

    buffer[11] = 0x02; // Flags de conexão (Clean Session)

    buffer[12] = 0x00;
    buffer[13] = 0x3C; // Keep Alive de 60 segundos

    int index = 14;

    // Client ID
    buffer[index++] = (client_id_len >> 8) & 0xFF;
    buffer[index++] = client_id_len & 0xFF;
    memcpy(&buffer[index], client_id, client_id_len);
    index += client_id_len;

    buffer_size = index; // Atualiza o tamanho total do buffer
    return buffer_size;
}
