#define MANDRAK_HEADER
typedef struct{
  int port;
  int verbose;
  struct in_addr *dest_ip; 
  struct sockaddr_in *if_adr;
}arguments;

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
  uint16_t hlen_resend; //HLEN and resend
  uint16_t window; //window size
  uint16_t crc; //standaard checksum
  uint16_t urgent_pnt; //Urgent Pointer
  uint32_t opt_pad; //options and paddding

}tcp_header;
void help(int argc, char** argv,int h); //function to display help page
arguments * parse_arguments(int argc, char **argv);
