#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) {
  if(argc<2){
    printf("invalid usage: use ./pa1 <ip address>\n");
    return -1;
  }

  printf("%s",argv[1]);

  struct sockaddr_in serv_addr;
  if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
  {
    printf("invalid ip address");
    return -1;
  }
  printf("done");

  return 0;
}
