#include "MqttPacket.h"

MqttPacket::MqttPacket() : buffer_size(0),
                           topic_size(0),
                           payload_size(0),
                           packet_id(0)
{
}

MqttPacket::MqttPacket(const char *t, const char *p) : MqttPacket()
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
    int remaining_length = 2 + topic_size + payload_size; // Tamanho correto do pacote

    if (remaining_length > 127)
    {
        return -2; // Erro: tamanho excede o limite de um único byte no campo Remaining Length
    }

    // buffer[0] = 0x30;             // MQTT PUBLISH header (QoS 0, no retain)
    buffer[0] = 0x32;             // MQTT PUBLISH header (QoS 1)
    buffer[1] = remaining_length; // Remaining length

    buffer[2] = (topic_size >> 8) & 0xFF; // Topic Length MSB
    buffer[3] = topic_size & 0xFF;        // Topic Length LSB

    memcpy(&buffer[4], topic, topic_size); // Copia o nome do tópico

    int msg_id = 10;
    buffer[4 + topic_size] = (msg_id >> 8) & 0xFF; // Topic Length MSB
    buffer[4 + topic_size + 1] = msg_id & 0xFF;    // Topic Length LSB

    memcpy(&buffer[4 + topic_size + 2], payload, payload_size); // Copia o payload

    buffer_size = 4 + topic_size + payload_size; // Atualiza o tamanho total do buffer
    return buffer_size;
}

int MqttPacket::create_subscribe_packet(uint16_t id)
{
    packet_id = id;
    int remaining_length = 2 + 2 + topic_size + 1; // ID do pacote (2 bytes) + Tamanho do tópico (2 bytes) + Tópico + QoS (1 byte)

    if (remaining_length > 127)
    {
        return -2; // Erro: tamanho excede o limite de um único byte no campo Remaining Length
    }

    buffer[0] = 0x82;             // MQTT SUBSCRIBE header
    buffer[1] = remaining_length; // Remaining length

    buffer[2] = (packet_id >> 8) & 0xFF; // Packet ID MSB
    buffer[3] = packet_id & 0xFF;        // Packet ID LSB

    buffer[4] = (topic_size >> 8) & 0xFF;  // Topic Length MSB
    buffer[5] = topic_size & 0xFF;         // Topic Length LSB
    memcpy(&buffer[6], topic, topic_size); // Copia o nome do tópico

    buffer[6 + topic_size] = 0x00; // QoS 0 (Entrega no máximo uma vez)

    buffer_size = 7 + topic_size; // Atualiza o tamanho total do buffer
    return buffer_size;
}

int MqttPacket::create_connect_packet(char *client_id)
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

    uint16_t keep_alive = 600;
    buffer[12] = keep_alive >> 8;   // Keep Alive MSB
    buffer[13] = keep_alive & 0xFF; // Keep Alive LSB

    int index = 14;

    // Client ID
    buffer[index++] = (client_id_len >> 8) & 0xFF;
    buffer[index++] = client_id_len & 0xFF;
    memcpy(&buffer[index], client_id, client_id_len);
    index += client_id_len;

    buffer_size = index; // Atualiza o tamanho total do buffer
    return buffer_size;
}
