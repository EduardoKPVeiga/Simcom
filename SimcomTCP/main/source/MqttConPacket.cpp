#include "MqttConPacket.h"

MqttConPacket::MqttConPacket(string c_id) : MqttPacket(), client_id(c_id)
{
}

MqttConPacket::~MqttConPacket()
{
}

void MqttConPacket::create_packet()
{
    if (client_id.empty())
    {
        return;
    }

    int client_id_len = client_id.length();

    if (client_id_len > 23)
    {
        // Limite padrão para muitos brokers
        return;
    }

    int remaining_length = 12 + (2 + client_id_len);

    if (remaining_length > 127)
    {
        return; // Tamanho excede o limite simples do protocolo MQTT
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
    memcpy(&buffer[index], client_id.c_str(), client_id_len);
    index += client_id_len;

    buffer_size = index; // Atualiza o tamanho total do buffer
}