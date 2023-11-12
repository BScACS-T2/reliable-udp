#ifndef CLIENT_PACKET_CONFIG_H
#define CLIENT_PACKET_CONFIG_H

#include <stdint.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <printf.h>
#include <string.h>

typedef struct header
{
    uint32_t                    seq_number;
    uint32_t                    ack_number;
    uint8_t                     flags;
    uint8_t                     window_size;
    struct timeval              tv;
    struct sockaddr_storage     src_ip;
    struct sockaddr_storage     dst_ip;
} header;

typedef struct packet
{
    struct header   hd;
    char            data[512];
} packet;

typedef struct sent_packet
{
    struct packet   pt;
    uint32_t        expected_ack_number;
    uint8_t         is_packet_empty;
} sent_packet;

int create_ack_packet(const struct packet *client_packet, struct packet *ack_packet);
int create_ack_number(uint32_t seq_number, uint32_t data_size, uint32_t *ack_number);

#endif //CLIENT_PACKET_CONFIG_H
