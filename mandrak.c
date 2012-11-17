#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>

#include "mandrak.h"


int main(int argc, char **argv){
  if(getuid()){
    printf("U root?\n");
    exit(-1);
  }
  parse_arguments(argc, argv);
  return 0;
}

void parse_arguments(int argc, char **argv){
  if(argc <= 3){
    help(argc,argv,1);
  }
    
  int port_flag = 0;
  int ip_addr_flag = 0;
  int int_flag = 0;
  int verbose_flag = 0;
  int c = 0;
  
  char *dest_ip = NULL;
  char *dest_port = NULL;
  char *int_f = NULL;
  arguments *arg = (arguments*) malloc(sizeof(arguments));
  
  int opterr = 0;
  while((c = getopt(argc, argv,"d:p:i:vh")) != -1){
    switch(c){
    case 'd':
      ip_addr_flag = 1;
      dest_ip = optarg;
      break;
    case 'p':
      port_flag = 1;
      dest_port = optarg;
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
      verbose_flag = 1;
      break;
    case 'h':
      help(argc,argv,1);
      break;
    default:
      help(argc,argv,0);
    }
  }
  
  printf("%d \n", port_flag);
  printf("%d \n", ip_addr_flag);
  printf("%d \n", int_flag);
  printf("%d \n", verbose_flag);
  
  
  
  

  free(arg);
}

void help(int argc, char **argv, int h){
  if(h!=0)   
    printf("Mandrak |<Copyleft>| Sander Demeester <sanderd@zeus.ugent.be>\n");
  printf("Usage: %s -d <destination ip> -p <port> -i <network interface> [-v <verbose>]\n", argv[0]);
  
  exit(-1);
}
