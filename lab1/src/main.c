#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  //check command line options
  /*
  if(argc<2){
    fprintf(stderr, "invalid usage: use ./pa1 <ip address>\n");
    return -1;
  }
  */

  //printf("%s\n",argv[1]);

  struct sockaddr_in serv_addr;
  struct hostent *server;
  int sockfd;

  //outgoing messages
  //currently example, change to parameter for final
  char *msgGet = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
  char *msgHead = "HEAD /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";

  /*
  if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
  {
    fprintf(stderr, "invalid ip address\n");
    return -1;
  }
  */

  //make socket
  printf("making socket");
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd<0){
    fprintf(stderr, "socket not opened");
    return -1;
  }

  //get server host info
  printf("getting host");
  server = gethostbyname("www.example.com");
  //server = gethostbyname(argv[1]);

  if(server==NULL){
    fprintf(stderr, "host not found");
    return -1;
  }

  //add dest host info
  /*
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = 80; //for HTTP
  bcopy((char *)server->serv_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  */
  printf("copying host info");
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = 80;
  memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

  //connect socket
  printf("connecting socket");
  if(connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0){
    fprintf(stderr, "sockfd connection failed");
    return -1;
  }



  printf("done");

  return 0;
}
