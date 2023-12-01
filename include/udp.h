#ifndef LIBUDP_UDP_H
#define LIBUDP_UDP_H 1

#include <stddef.h>
#include <stdint.h>

#define IP_UDP_PROTOCOL 0x11

typedef uint32_t udp_ipv4;

typedef struct _udp_header
{
  uint16_t source_port;
  uint16_t destination_port;
  uint16_t length;
  uint16_t checksum;
} udp_header_t;

typedef struct _udp_pseudo_header4
{
  udp_ipv4 source_address;
  udp_ipv4 destination_address;
  uint8_t zero;
  uint8_t protocol;
  uint16_t length;
} udp_pseudo_header4_t;

typedef struct _udp_packet
{
  udp_header_t header;
  char data[];
} udp_packet_t;

udp_packet_t *udp_packet_init (void *buffer, size_t buffer_len,
                               uint16_t source_port,
                               uint16_t destination_port);

void udp_packet_finalize4 (udp_packet_t *packet, udp_ipv4 source_address,
                           udp_ipv4 destination_address);

udp_packet_t *udp_packet_read4 (void *buffer, size_t len,
                                udp_ipv4 source_address,
                                udp_ipv4 destination_address);
#endif
