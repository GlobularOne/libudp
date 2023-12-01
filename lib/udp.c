#include <udp.h>

#ifdef __LITTLE_ENDIAN__

#define htons(n) __builtin_bswap16 (n)
#define htonl(n) __builtin_bswap32 (n)
#define ntohs(n) __builtin_bswap16 (n)
#define ntohl(n) __builtin_bswap32 (n)

#else

#define htons(n) (n)
#define htonl(n) (n)
#define ntohs(n) (n)
#define ntohl(n) (n)

#endif

static uint32_t
ip_checksum_calculate (uint32_t sum, void *buffer, size_t buffer_len)
{
  uint16_t *cbuf = (uint16_t *)buffer;
  while (buffer_len >= 2)
    {
      sum += *cbuf++;
      buffer_len -= 2;
    }
  if (buffer_len == 1)
    {
      sum += *(uint8_t *)buffer;
    }
  return sum;
}

static uint32_t
ip_checksum_finalize (uint32_t sum)
{
  while (sum >> 16)
    {
      sum = (sum & 0xFFFF) + (sum >> 16);
    }
  if (sum == 0)
    {
      return 0xFFFF;
    }
  return sum;
}

udp_packet_t *
udp_packet_init (void *buffer, size_t buffer_len, uint16_t source_port,
                 uint16_t destination_port)
{
  if (buffer_len < sizeof (udp_header_t) || buffer == NULL
      || buffer_len >= 65507)
    {
      return NULL;
    }
  udp_packet_t *packet = (udp_packet_t *)buffer;
  packet->header.length = htons ((uint16_t)buffer_len);
  packet->header.checksum = 0;
  packet->header.source_port = htons (source_port);
  packet->header.destination_port = htons (destination_port);
  return packet;
}

void
udp_packet_finalize4 (udp_packet_t *packet, udp_ipv4 source_address,
                      udp_ipv4 destination_address)
{
  if (packet == NULL)
    {
      return;
    }
  udp_pseudo_header4_t pseudo_header;
  pseudo_header.length = packet->header.length;
  pseudo_header.source_address = htonl (source_address);
  pseudo_header.destination_address = htonl (destination_address);
  pseudo_header.zero = 0;
  pseudo_header.protocol = IP_UDP_PROTOCOL;
  uint32_t checksum = ip_checksum_calculate(checksum, &pseudo_header, sizeof(pseudo_header));
  checksum = ip_checksum_calculate(checksum, packet, ntohs(packet->header.length));
  checksum = ip_checksum_finalize(checksum);
  packet->header.checksum = htons(checksum);
}

udp_packet_t *
udp_packet_read4 (void *buffer, size_t len, udp_ipv4 source_address,
                  udp_ipv4 destination_address)
{
    if(buffer == NULL || len < sizeof(udp_header_t))
    {
        return NULL;
    }
    udp_packet_t* packet = (udp_packet_t*)buffer;
    if(ntohs(packet->header.length) > len)
    {
        /* Packet is truncated */
        return NULL;
    }
    /* Checksum is optional if UDP/IPv4 so just return it */
    packet->header.checksum = 0;
    return packet;
}


