#define MANDRAK_HEADER
typedef struct{
  int port;
  struct in_addr dest_ip; 
  struct sockaddr_in if_adr;
}arguments;
void help(int argc, char** argv); //function to display help page
