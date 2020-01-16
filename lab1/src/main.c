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
      isH = 1;
      printf("has -h\n");
    }
  }
  char param1[param1Length];
  strcpy(param1,argv[1]);
  char param2[param2Length];
  strcpy(param2,argv[2]);


  printf("splitting param2\n");
  char *tok1 = strtok(param2,":");
  char *ipaddr;
  char *path;
  char *port;
  char *temp;

  printf("token1: %s\n",tok1 );
  //printf("%s\n",strtok(param2,"/"));
  if(strlen(tok1)==param2Length){//there is no extra port specified
    printf("no port\n");
    temp = strtok(tok1,"/");
    ipaddr = (char*)malloc((strlen(temp)+1)*(sizeof(char)));
    strcpy(ipaddr,temp);
    printf("ipaddr: %s\n",ipaddr);

    temp = strtok(NULL,"/");
    path = (char*)malloc((strlen(temp)+1)*(sizeof(char)));
    strcpy(path,temp);
    printf("path: %s\n\n",path);

    port = NULL;
  }
  else{
    printf("is extra port\n\n");
    ipaddr = tok1;

    ipaddr = (char*)malloc((strlen(tok1)+1)*(sizeof(char)));
    strcpy(ipaddr,tok1);
    printf("ipaddr: %s\n",ipaddr);

    temp = strtok(NULL,"/");
    port = (char*)malloc((strlen(temp)+1)*(sizeof(char)));
    strcpy(port,temp);
    printf("port: %s\n",port);

    temp = strtok(NULL,"/");
    path = (char*)malloc((strlen(temp)+1)*(sizeof(char)));
    strcpy(path,temp);
    printf("path: %s\n\n",path);
  }
  printf("param2Length: %i\n",strlen(param2));
  printf("param2: %s\n\n",param2);

  //printf("stuff: %s\n%s\n%s\n", ipaddr,path,port);

  //int tempLength = 4+strlen(path);
  //char temp[100];
  //strcat(temp,"/");
  //strcat(temp,path);
  //printf("new path: %s\n",temp);
  //char *ipaddr =


  //char *ipaddr = "8.8.8.8";

  //outgoing messages
  //currently example, change to parameter for final
  //char *msgGet = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
  //char *msgHead = "HEAD /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
  char *msgGet = malloc(1000*sizeof(char));
  strcpy(msgGet,"GET /");
  strcat(msgGet,path);
  strcat(msgGet," HTTP/1.1\r\nHost: ");
  strcat(msgGet,param1);
  strcat(msgGet,"\r\n\r\n");

  char *msgHead = malloc(1000*sizeof(char));
  strcpy(msgHead,"HEAD /");
  strcat(msgHead,path);
  strcat(msgHead," HTTP/1.1\r\nHost: ");
  strcat(msgHead,param1);
  strcat(msgHead,"\r\n\r\n");



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

  serv_addr.sin_family = AF_INET;
  if(port==NULL){
    serv_addr.sin_port = htons(80); //port for HTTP if none specified
  }
  else{
    serv_addr.sin_port = htons(atoi(port)); //use provided port
  }
  printf("ipaddr: %s\n",ipaddr);
  serv_addr.sin_addr.s_addr = inet_addr(ipaddr);
  //debug manual ip
  //serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34");


  //connect socket
  /*
  printf("\nconnecting socket\n");
  printf("IP: %s\n", ipaddr);
  if(port!=NULL){
    printf("port: %s\n", port);
  }
  else{
    printf("port (none given): 80\n");
  }
  */

  if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
    fprintf(stderr, "sockfd connection failed\n");
    return -1;
  }

  //send message

  int msgstatus;
  printf("sending message\n");
  char *toSend = malloc(strlen(msgHead)*sizeof(char));

  if(isH==1){
    strcpy(toSend,msgHead);
  }
  else{
    strcpy(toSend,msgGet);
  }
  printf("message: %s\n", toSend);

  msgstatus = write(sockfd,toSend,strlen(msgGet));
  if (msgstatus<0) {
    fprintf(stderr, "message failed to send\n");
  }
  printf("receiving reply\n");
  msgstatus = write(sockfd,toSend,strlen(msgGet));

  //i send it again because theres some issues with head with only one send
  //¯\_(ツ)_/¯
  //it just works
  char reply[4096];
  msgstatus = read(sockfd,reply,4095);
  if (msgstatus<0) {
    fprintf(stderr, "failed to receive reply\n");
  }
  printf("%s\n",reply );
  printf("done\n");

  return 0;
}
