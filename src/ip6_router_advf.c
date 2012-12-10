#ifndef GENERIC
#include "header/generic.h"
#endif

#include "header/ip6_router_advf.h"

void setup_router_ra_flood(){
  unsigned char* MAC = (unsigned char*) malloc(sizeof(unsigned char*)*6);
  unsigned char* ip6 = (unsigned char*) malloc(sizeof(unsigned char*)*16);
  unsigned char *routerip6 = NULL;
  unsigned char *route6 = NULL;
  unsigned char *MAC6 = MAC;
  
  unsigned char* packet = NULL;
  int size = 64;
  int mtu = 1500;
  
    
}
