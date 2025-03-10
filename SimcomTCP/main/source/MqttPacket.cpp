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
    int remaining_length = 2 + 6 + strlen(client_id) + 2 + strlen(username) + 2 + strlen(password); // Protocol name + Protocol level + Connect flags + Keep alive + Client ID + Username + Password

    buffer[0] = 0x10; // MQTT CONNECT header
    buffer[1] = remaining_length;

    buffer[2] = 0x00; // Protocol name length MSB
    buffer[3] = 0x04; // Protocol name length LSB
    buffer[4] = 'M';  // Protocol name
    buffer[5] = 'Q';  // Protocol name
    buffer[6] = 'T';  // Protocol name
    buffer[7] = 'T';  // Protocol name

    buffer[8] = 0x04; // Protocol level (4)

    buffer[9] = 0x02;  // Connect flags (Clean session)
    buffer[10] = 0x00; // Keep alive MSB
    buffer[11] = 0x3C; // Keep alive LSB (60 seconds)

    buffer[12] = 0x00;                                 // Client ID length MSB
    buffer[13] = strlen(client_id);                    // Client ID length LSB
    memcpy(&buffer[14], client_id, strlen(client_id)); // Client ID

    buffer[14 + strlen(client_id)] = 0x00;                               // Username length MSB
    buffer[15 + strlen(client_id)] = strlen(username);                   // Username length LSB
    memcpy(&buffer[16 + strlen(client_id)], username, strlen(username)); // Username

    buffer[16 + strlen(client_id) + strlen(username)] = 0x00;                               // Password length MSB
    buffer[17 + strlen(client_id) + strlen(username)] = strlen(password);                   // Password length LSB
    memcpy(&buffer[18 + strlen(client_id) + strlen(username)], password, strlen(password)); // Password

    buffer_size = 18 + strlen(client_id) + strlen(username) + strlen(password); // Total packet size
    return buffer_size;
}