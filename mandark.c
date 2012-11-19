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

int main(int argc, char **argv){
  if(getuid()){
    printf("U root?\n");
    exit(-1);
  }
  
  arguments *arg = parse_arguments(argc, argv);

  pthread_t *syn_engine    = (pthread_t*) malloc(sizeof(pthread_t));
  pthread_t *packet_engine = (pthread_t*) malloc(sizeof(pthread_t));
  pthread_t *ani           = (pthread_t*) malloc(sizeof(pthread_t));
  
  pthread_create(packet_engine, NULL, process_incoming_packets,(void*)arg);
  pthread_create(syn_engine,NULL,process_syn,(void*)arg);
  pthread_create(ani,NULL,ani_f,NULL);

  pthread_join(*syn_engine,NULL);
  pthread_join(*ani,NULL);
  pthread_join(*packet_engine,NULL);
  
  return 0;
}

arguments *parse_arguments(int argc, char **argv){
  if(argc <= 4){
    help(argc,argv,1);
  }
    
  int c = 0;
  
  arguments *arg = (arguments*) malloc(sizeof(arguments));
  arg->dest_ip = (struct in_addr*) malloc(sizeof(struct in_addr));
  arg->if_adr =  (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  
  while((c = getopt(argc, argv,"d:p:i:vh")) != -1){
    switch(c){
    case 'd':
      if(inet_aton(optarg,arg->dest_ip) == 0){
	printf("Invalid IP-adr\n");
	help(argc, argv,0);
	exit(-1);
      }
      break;
    case 'p':
      arg->port = atoi(optarg);
      break;
    case 'i':{
      int f_device;
      struct ifreq netdevice; //low level acces to network devices
      f_device = socket(AF_INET,SOCK_DGRAM,0);
      netdevice.ifr_addr.sa_family = AF_INET;
      strncpy(netdevice.ifr_name,optarg,IFNAMSIZ-1);

      //Get interface adr for protocol family
      if(ioctl(f_device, SIOCGIFADDR,&netdevice)){ 
	printf("Device not found\n");
	help(argc, argv,0);
      }
      close(f_device);
      arg->if_adr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
      arg->if_adr->sin_family = AF_INET;
      arg->if_adr->sin_addr = ((struct sockaddr_in*)&netdevice.ifr_addr)->sin_addr;
      break;
    }
    case 'v':
      arg->verbose = 1;
      break;
    case 'h':
      help(argc,argv,1);
      break;
    default:
      help(argc,argv,0);
    }
  }
  return arg;
}

void help(int argc, char **argv, int h){
  if(h!=0)   
    printf("Mandark |<Copyleft>| Sander Demeester <sanderd@zeus.ugent.be>\n");
  printf("Usage: %s -d <destination ip> -p <port> -i <network interface> [-v <verbose>]\n", argv[0]);
  
  exit(-1);
}
void *ani_f(){
  int pos = 0;
  int i;
  int inc = 1;
  int aantal = 0;
  while(1){
    printf("\033[2K");
    printf("|");
    for(i = 0; i < pos; i++)
      printf(" ");
    pos+=inc;
    if(pos == 40) inc = -1;
    if(pos == 0) inc = 1;
    printf("HaHaHa - HaHaHa");
    for(i = 0; i <= 39-pos;i++) printf(" ");
    printf("|<%d syn send>\r",aantal*100);
    fflush(stdout);
    usleep(500*1000);
    aantal++;
  }
}
