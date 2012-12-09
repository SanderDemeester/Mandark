#ifndef GENERIC
#include "header/generic.h"
#endif

#include "header/tcp_wrap.h"

void *process_incoming_packets(void*ptr){
  struct tcp_options *tcp_f = (struct tcp_options*) calloc(1,sizeof(struct tcp_options));
  arguments *arg = (arguments*)ptr;
  int listen_socket = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
  int ack_socket = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);

  if(listen_socket < 0){
    printf("Error while creating RAW_SOCKET to process incoming packets\n");
    exit(-1);
  }
  
  //We need a local socket to proces data
  
  struct sockaddr_in *me = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  me->sin_family = AF_INET;
  me->sin_addr.s_addr = INADDR_ANY;
  
  bind(listen_socket,(struct sockaddr*)me,sizeof(*me));
  unsigned char b[500]; //alocate on the stack for this thread.
  while(1){
    int n = recv(listen_socket,b,500,0); //listen to everything adr to us
    
    ip_header *iph = (ip_header*)b;
    tcp_header *tcph = (tcp_header*)(b + 4 * (iph->version_ihl & 0x0F)); //fix ip part of header 
    if(iph->src_adr == arg->dest_ip->s_addr && iph->proto == 6){
      //Go johnny Go
      /* When we are here, there are different senarios that can happen. */
      /* 	1. We need to finish the connection, blowing away the stateless-niss on the otherside */
      /* 	2. We need to keep the connection alive */

      struct in_addr src_addr;
      src_addr.s_addr = iph->src_adr;
      
      tcp_f->urg = tcph->hlen_re_flag & htons(0x0020);
      tcp_f->psh = tcph->hlen_re_flag & htons(0x0008);
      tcp_f->rst = tcph->hlen_re_flag & htons(0x0004);
      tcp_f->syn = tcph->hlen_re_flag & htons(0x0002);
      tcp_f->ack = tcph->hlen_re_flag & htons(0x0010);
      tcp_f->fin = tcph->hlen_re_flag & htons(0x0001);

      process_ack(b,(arguments*)ptr,&ack_socket);
    }
  }
}
void *process_syn(void*ptr){
  
  arguments *arg = (arguments*)ptr;
  int syn_raw_socket = socket(AF_INET, SOCK_RAW,IPPROTO_TCP); //creating raw socket to send syn packets
  tcp_header *tcph = (tcp_header*) malloc(sizeof(tcp_header)); //our tcp header
  
  struct sockaddr_in *connection_dest_addr = (struct sockaddr_in*) calloc(1,sizeof(struct sockaddr_in)); //sockadr for destination, get info from arg.
  
  srand(time(NULL)); //seeding random function for seq numbers.
  
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
  
  //Fill in destination socket information.
  connection_dest_addr->sin_family = AF_INET;
  connection_dest_addr->sin_addr = *arg->dest_ip;
  
  while(1){
  tcph->src_port = (rand() & 0xFFFF); //our port is random
  tcph->dst_port = htons(arg->port); //our dest port

  tcph->seq = htonl(rand()); //our init seq is random
  tcph->ack = 0; //first syn is 0
  tcph->hlen_re_flag = 0; //sign ext
  tcph->hlen_re_flag |= htons(0x6000); //data field
  tcph->hlen_re_flag |= htons(0x0002); //set syn flag
  
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
  usleep(10*1000); //5ms 
  }
}
void process_ack(unsigned char*b,arguments*arg, int *socket_ack){
#ifdef debug
  printf("incoming ack\n");
#endif
  
  struct sockaddr_in *connection_dest_addr = (struct sockaddr_in*) calloc(1,sizeof(struct sockaddr_in));
  connection_dest_addr->sin_family = AF_INET;
  connection_dest_addr->sin_addr = *arg->dest_ip;

  unsigned char *ack_reply = (unsigned char*) malloc(sizeof(unsigned char)*sizeof(tcp_header));
  
  ip_header *iph          = (ip_header*)b;
  tcp_header *tcph_synack = (tcp_header*)(b + 4*(iph->version_ihl & 0x0F)); //header for syn-ack
  tcp_header *tcph_ack    = (tcp_header*)ack_reply; //header for ack
  
  if(*socket_ack < 0){
    printf("Error while creating RAW SOCKET for ACK\n");
    exit(-1);
  }
  
  if(bind(*socket_ack,(struct sockaddr*)arg->if_adr,
	  sizeof(struct sockaddr_in)) == -1){
    printf("Fear for binding?\n");
    exit(-1);
  }

  tcph_ack->src_port = tcph_synack->dst_port; //we used a random port, the sever is so friendly to provid it to us (stateless u say?)
  tcph_ack->dst_port = tcph_synack->src_port; //where did U comefrom please?
  tcph_ack->ack = htonl(ntohl(tcph_synack->seq)+1);
  tcph_ack->seq = tcph_synack->ack;
  
  tcph_ack->hlen_re_flag = 0; //sign ext, zero out flags && length
  tcph_ack->hlen_re_flag |= htons(0x6000); //lengte
  tcph_ack->hlen_re_flag |= htons(0x0010); //ack flag

  tcph_ack->window = 0; //stall the other end
  tcph_ack->urgent_pnt = 0;
  tcph_ack->opt_pad = 0;
  
  crc_checksum(ack_reply,sizeof(tcp_header),&arg->if_adr->sin_addr,&connection_dest_addr->sin_addr);
  if(sendto(*socket_ack,ack_reply,sizeof(tcp_header),0,(struct sockaddr*)connection_dest_addr,sizeof(struct sockaddr_in))==-1){
      printf("Error while sending SYNACK\n");
    }

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
