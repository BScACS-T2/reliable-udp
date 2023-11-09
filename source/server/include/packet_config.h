#ifndef CLIENT_PACKET_CONFIG_H
#define CLIENT_PACKET_CONFIG_H

#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <printf.h>

typedef struct header
{
    uint32_t        sequence_number;
    uint32_t        acknowledgment_number;
    uint8_t         flags;
    uint8_t         window_size;
    struct timeval  tv;
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
    struct timeval  sent_tv;
} sent_packet;

int recreate_packet(const char *buffer, struct packet *pt);
int create_ack_number(uint32_t seq_number, uint32_t data_size, uint32_t *ack_number);

#endif //CLIENT_PACKET_CONFIG_H