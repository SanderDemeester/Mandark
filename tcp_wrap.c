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

void process_incoming_packets(){
}
void process_syn(arguments *arg){

  int syn_raw_socket = socket(AF_INET, SOCK_RAW,IPPROTO_TCP);
  tcp_header *tcph = (struct tcp_header*) malloc(sizeof(tcp_header));
  struct sockaddr_in *connection_dest_addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));

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

  tcph->window = 1000; //default window size (handshake syn)
  tcph->urgent_pnt = 0; //urgent flag zero
  
  tcph->opt_pad = 0; //sign ext
  

  
}
void process_ack(unsigned char packet){
}
