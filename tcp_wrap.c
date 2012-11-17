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
  
  
  
}
void process_ack(unsigned char packet){
}
