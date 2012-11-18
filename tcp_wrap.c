#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "tcp_wrap.h"

void *process_incoming_packets(void*ptr){
  struct tcp_options tcp_f;
  int listen_socket = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
  unsigned char *packet_buffer = (unsigned char*) malloc(sizeof(unsigned char)*1000);
  
  if(listen_socket < 0){
    printf("Error while creating RAW_SOCKET to process incoming packets\n");
    exit(-1);
  }
  
  //We need a local socket to proces data
  
  struct sockaddr_in *me = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  me->sin_family = AF_INET;
  me->sin_addr.s_addr = INADDR_ANY;
  
  bind(listen_socket,(struct sockaddr*)me,sizeof(*me));
  
  
 
}
void process_syn(arguments *arg){

  int syn_raw_socket = socket(AF_INET, SOCK_RAW,IPPROTO_TCP);
  tcp_header *tcph = (tcp_header*) malloc(sizeof(tcp_header));
  struct sockaddr_in *connection_dest_addr = (struct sockaddr_in*) calloc(1,sizeof(struct sockaddr_in));
  
  srand(time(NULL));
    if(syn_raw_socket < 0){
    printf("Error while creating RAW socket for syn sending\n");
    printf(".. Sorry\n");
    exit(-1);
  }

  //Ah yes, fear to bind/commit. 
  if(bind(syn_raw_socket,(struct sockaddr*)arg->if_adr,
	  sizeof(struct sockaddr_in)) == -1){
    printf("Error while trying to bind raw socket");
    if(arg->verbose)
      printf("Fear of binding? No problem, it happes to the best of us\n");
    exit(-1);
  }
  
  
  connection_dest_addr->sin_family = AF_INET;
  connection_dest_addr->sin_addr = *arg->dest_ip;
  
  //for test send just 1
  tcph->src_port = (rand() & 0xFFFF); //our port is random
  tcph->dst_port = htons(arg->port); //our dest port

  tcph->seq = htonl(rand()); //our init seq is random
  tcph->ack = 0; //first syn is 0
  tcph->hlen_re_flag = 0; //sign ext
  tcph->hlen_re_flag |= htons(0x0002); //set syn flag
  tcph->hlen_re_flag |= htons(0x6000); //data field

  tcph->window = 1000; //default window size (handshake syn)
  tcph->urgent_pnt = 0; //urgent flag zero
  
  tcph->opt_pad = 0; //sign ext

  crc_checksum((unsigned char*)tcph,sizeof(tcp_header),&arg->if_adr->sin_addr,
	       &connection_dest_addr->sin_addr);

  if(sendto(syn_raw_socket,tcph,sizeof(tcp_header),0,
		   (struct sockaddr*)connection_dest_addr,
	    sizeof(struct sockaddr_in)) == -1){
    printf("Error while sending SYN packet\n");
    exit(-1);
  }
  
}
void process_ack(unsigned char packet){
}

void crc_checksum(unsigned char *packet, unsigned int packet_len, 
		  struct in_addr *src, 
		  struct in_addr *dst){
  uint32_t sum = 0;

  uint32_t src_ip = ntohl(src->s_addr);
  uint32_t dst_ip = ntohl(dst->s_addr);
  int i;

  sum += (src_ip >> 16) & 0xFFFF;
  sum = (sum + (1 & (sum >> 16))) & 0xFFFF;
  
  sum += src_ip & 0x0000FFFF;
  sum = (sum + (1 & (sum >> 16))) & 0xFFFF;
  
  sum += (dst_ip >> 16) & 0xFFFF;
  sum = (sum + (1 & (sum >> 16))) & 0xFFFF;

  sum += dst_ip & 0x0000FFFF;
  sum = (sum + (1 & (sum >> 16))) & 0xFFFF;

  sum += 0x0006;
  sum = (sum + (1 & (sum >> 16))) & 0xFFFF;
  
  //len
  sum += packet_len;
  sum = (sum + (1 & (sum >> 16))) & 0xFFFF;

  tcp_header *tcph = (tcp_header*) packet;
  tcph->crc = 0;

  for(i = 0; i < packet_len / 2; i++){
    uint16_t b = (packet[i*2] << 8) | packet[i*2+1]; //kdb, yes i know
    sum += b;
    sum = (sum + (1 & (sum >> 16))) & 0xFFFF;
  }
  
  //packet algin
  if(packet_len % 2 == 1){
    uint16_t b = packet[packet_len-1] << 8;
    sum += b;
    sum = (sum + (1 & (sum >> 16))) & 0xFFFF;
  }
  tcph->crc = htons(~sum);
  
}
