#define MANDRAK_HEADER
typedef struct{
  int port;
  int verbose;
  struct in_addr *dest_ip; 
  struct sockaddr_in *if_adr;
}arguments;

void help(int argc, char** argv,int h); //function to display help page
arguments *parse_arguments(int argc, char **argv);
