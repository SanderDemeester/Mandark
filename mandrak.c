#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>

#include "mandrak.h"


int main(int argc, char **argv){
  return 0;
}

void parse_arguments(int argc, char **argv){
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
  while((c = getopt(argc, argv,"d:p:i:v")) != -1){
    switch(c){
    case 'd':
      ip_addr_flag = 1;
      dest_ip = optarg;
      break;
    case 'p':
      port_flag = 0;
      dest_port = optarg;
      break;
    case 'i':
      int_flag = 1;
      int_f = optarg;
      break;
    }
  }
  free(arg);
}

