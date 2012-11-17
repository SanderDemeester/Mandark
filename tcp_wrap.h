#include "mandrak.h"
typedef struct{
  unsigned char version_ihl; //version
  unsigned char tos; //type of service
  uint16_t len; //length 
  uint32_t id_flag_frag; //identification & flags & flag offset
  unsigned char ttl; //time to live
  unsigned char proto; //protoco field
  uint16_t crc; //standaard checksum
  uint32_t src_adr; //src adr
  uint32_t dest_adr; //dest adr
  uint32_t options; //if IHL > 5
}ip_header;

typedef struct{
  uint16_t src_port; //source port
  uint16_t dst_port; //dest port
  uint32_t seq; //seq number
  uint32_t ack; //ack number
  uint16_t hlen_re_flag; //HLEN and reserved & flags
  uint16_t window; //window size
  uint16_t crc; //standaard checksum
  uint16_t urgent_pnt; //Urgent Pointer
  uint32_t opt_pad; //options and paddding
}tcp_header;


struct tcp_options{
  int urg; //urgent flag
  int ack; //ack
  int psh; //push ... the button
  int rst; //reset 
  int syn; //syc
  int fin; //fin
};

void crc_checksum(unsigned char *packet, unsigned int packet_len, 
		  struct in_addr *src, 
		  struct in_addr *dst);

void *process_incoming_packets(void*ptr);
void process_syn(arguments *arg);
void process_ack(unsigned char packet);
