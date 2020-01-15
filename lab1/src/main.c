#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[]) {
  fprintf(stderr,"starting\n");
  //check command line options


  if(argc<3){
    fprintf(stderr, "invalid usage: use ./pa1 <hostname> <URL> <-h (optional)>\n");
    return -1;
  }

  int param1Length;
  int param2Length;
  int isH;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int sockfd;
  int desPort;

  printf("%s\n",argv[1]);
  param1Length = strlen(argv[1]);
  printf("%s\n",argv[2]);
  param2Length = strlen(argv[2]);
  if(strcmp(argv[3],"-h")==0){
    int isH = 1;
    printf("has -h\n");
  }
  char param1[param1Length];
  strcpy(param1,argv[1]);
  char param2[param2Length];
  strcpy(param2,argv[2]);

  //printf("%s\n%s\n",param1,param2);


  //char *ipaddr = "8.8.8.8";

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
  printf("making socket\n");
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd<0){
    fprintf(stderr, "socket not opened\n");
    return -1;
  }

  /*
  //get server host info
  printf("getting host\n");
  server = gethostbyname("www.example.com");
  //server = gethostbyname(argv[1]);

  if(server==NULL){
    fprintf(stderr, "host not found\n");
    return -1;
  }
  */


  printf("copying host info\n");
  //memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  if(desPort==NULL){
    serv_addr.sin_port = htons(80); //port for HTTP if none specified
  }
  else{
    serv_addr.sin_port = htons(80);
  }

  serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34");
  //memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);


  //connect socket
  printf("connecting socket\n");
  //printf("%i\n",serv_addr.sin_addr.s_addr);
  if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
    fprintf(stderr, "sockfd connection failed\n");
    return -1;
  }

  //send message
  int msgstatus;
  printf("sending message\n");
  msgstatus = write(sockfd,msgGet,strlen(msgGet));
  if (msgstatus<0) {
    fprintf(stderr, "message failed to send\n");
  }
  printf("receiving reply\n");
  char reply[4096];
  msgstatus = read(sockfd,reply,4095);
  if (msgstatus<0) {
    fprintf(stderr, "failed to receive reply\n");
  }
  printf("%s\n",reply );
  printf("done\n");

  return 0;
}
