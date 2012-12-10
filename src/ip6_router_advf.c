#ifndef GENERIC
#include "header/generic.h"
#endif

#include "header/ip6_router_advf.h"

void setup_router_ra_flood(unsigned char*if6){
  if(if6 == NULL)
    exit(-1);
  unsigned char* MAC = (unsigned char*) calloc(6,sizeof(unsigned char*));
  unsigned char* ip6 = (unsigned char*) calloc(16,sizeof(unsigned char*));
  unsigned char *routerip6 = (unsigned char*) calloc(16,sizeof(unsigned char*));
  unsigned char *route6 = (unsigned char*) calloc(16,sizeof(unsigned char*));
  unsigned char *MAC6 = MAC;
  unsigned char* packet = NULL;
  
  ipv6_header *ip6h = NULL;
  int size = 64;
  int mtu = 1500;
  

  MAC[0] = 0x00; //first byte of mac
  MAC[1] = 0x18; //second byte of mac
  
  ip6[0] = 0xfe; //first  byte of link local prefix
  ip6[1] = 0x80; //second byte of link local prefix
  ip6[8] = 0x02; //follow algorite to generate soliticed node multicast adres
  ip6[9] = MAC[1]; 
  ip6[11] = 0xff;
  ip6[12] = 0xfe;
  

  routerip[0] = 0x26; //first byte iana allocated (random)
  routerip[1] = 0x0c; //second byte iana allocated (random)
}
