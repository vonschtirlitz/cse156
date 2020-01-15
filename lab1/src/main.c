#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) {
  //check command line options
  /*
  if(argc<2){
    fprintf(stderr, "invalid usage: use ./pa1 <ip address>\n");
    return -1;
  }
  */

  //printf("%s\n",argv[1]);

  struct sockaddr_in dest_addr;
  struct hostent *destination;
  int sock;

  //outgoing messages
  //currently example, change to parameter for final
  char *msgGet = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
  char *msgHead = "HEAD /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";

  if(inet_pton(AF_INET, argv[1], &dest_addr.sin_addr)<=0)
  {
    fprintf(stderr, "invalid ip address\n");
    return -1;
  }

  //make socket
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if(sock<0){
    fprintf(stderr, "socket not opened");
    return -1;
  }

  //get destination host info
  destination = gethostbyname("www.example.com");
  //destination = gethostbyname(argv[1]);

  if(destination==NULL){
    fprintf(stderr, "host not found");
    return -1;
  }

  //add dest host info
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = 80; //for HTTP
  bcopy((char *)destination->dest_addr, (char *)&dest_addr.sin_addr.s_addr, destination->h_length);

  //connect socket
  if(connect(sock,&dest_addr,sizeof(dest_addr))) < 0{
    fprintf(stderr, "sock connection failed");
    return -1;
  }



  printf("done");

  return 0;
}
