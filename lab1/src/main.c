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
  if(argc>3){
    if(strcmp(argv[3],"-h")==0){
      int isH = 1;
      printf("has -h\n");
    }
  }
  char param1[param1Length];
  strcpy(param1,argv[1]);
  char param2[param2Length];
  strcpy(param2,argv[2]);

  /*
  char *param2parts[3];
  int i =0;
  char *p = strtok(param2,":");
  while(p!=NULL){
    param2parts[i++]=p;
    p = strtok(NULL,":");
  }
  for (i = 0; i < 3; ++i)
        printf("%s\n", param2parts[i]);
        */



  printf("splitting param2\n");
  char *tok1 = strtok(param2,":");
  char *ipaddr,*path,*port;
  printf("token1: %s\n",tok1 );
  //printf("%s\n",strtok(param2,"/"));
  if(strlen(tok1)==param2Length){//there is no extra port specified
    printf("no port\n");
    ipaddr = strtok(tok1,"/");
    printf("ipaddr: %s\n",ipaddr);
    path = strtok(NULL,"/");
    printf("path: %s\n\n",path);
    port = NULL;
  }
  else{
    printf("is extra port\n\n");
    ipaddr = tok1;
    printf("ipaddr: %s\n",ipaddr);
    port = strtok(NULL,"/");
    printf("port: %s\n",port);
    path = strtok(NULL,"/");
    printf("path: %s\n\n",path);
  }
  printf("param2Length: %i\n",strlen(param2));
  printf("param2: %s\n\n",param2);

  //printf("stuff: %s\n%s\n%s\n", ipaddr,path,port);

  //int tempLength = 4+strlen(path);
  //char temp[tempLength];
  //strcat(temp,"/");
  //strcat(temp,path);
  //printf("new path: %s\n",temp);
  //char *ipaddr =


  //char *ipaddr = "8.8.8.8";

  //outgoing messages
  //currently example, change to parameter for final
  char *msgGet = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
  //char *msgGet = "GET "+temp+" HTTP/1.1\r\nHost: "+param1+"\r\n\r\n";
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
  if(port==NULL){

    serv_addr.sin_port = htons(80); //port for HTTP if none specified
  }
  else{
    //printf("port: %s\n", port);

    serv_addr.sin_port = htons(atoi(port));
  }
  printf("ipaddr: %s\n",ipaddr);
  //serv_addr.sin_addr.s_addr = inet_addr(ipaddr);
  serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34");

  //memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

  printf("hostinfocopied\n");

  //connect socket
  printf("\nconnecting socket\n");
  printf("IP: %s\n", ipaddr);
  if(port!=NULL){
    printf("port: %s\n", port);
  }
  else{
    printf("port (none given): 80\n");
  }
  //printf("%i\n",serv_addr.sin_addr.s_addr);
  if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
    fprintf(stderr, "sockfd connection failed\n");
    return -1;
  }

  //send message
  printf("message: %s\n", msgGet);
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
  //printf("%s\n",reply );
  printf("done\n");

  return 0;
}
