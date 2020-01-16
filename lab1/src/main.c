#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

//Nikita Usenko
//nusenko 1569296
//CSE156
//1/16/2020
//Lab1
//main.c

int main(int argc, char const *argv[]) {
  //check command line options
  if(argc<3){
    fprintf(stderr,
      "invalid usage: use ./pa1 <hostname> <URL> <-h (optional)>\n");
    return -1;
  }

  int param1Length;
  int param2Length;
  int isH;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int sockfd;
  int desPort;
  FILE * outfile;

  //printf("%s\n",argv[1]);
  param1Length = strlen(argv[1]);
  //printf("%s\n",argv[2]);
  param2Length = strlen(argv[2]);
  if(argc>3){
    if(strcmp(argv[3],"-h")==0){//has -h flag
      isH = 1;
    }
  }
  char param1[param1Length];
  strcpy(param1,argv[1]);
  char param2[param2Length];
  strcpy(param2,argv[2]);

  //split second argument to different parts with strtok
  char *tok1 = strtok(param2,":");
  char *ipaddr;
  char *path;
  char *port;
  char *temp;

  if(strlen(tok1)==param2Length){//there is no extra port specified
    temp = strtok(tok1,"/");
    ipaddr = (char*)malloc((strlen(temp)+1)*(sizeof(char)));
    strcpy(ipaddr,temp);

    temp = strtok(NULL,"/");
    path = (char*)malloc((strlen(temp)+1)*(sizeof(char)));
    strcpy(path,temp);

    port = NULL;
  }
  else{
    ipaddr = tok1;

    ipaddr = (char*)malloc((strlen(tok1)+1)*(sizeof(char)));
    strcpy(ipaddr,tok1);

    temp = strtok(NULL,"/");
    port = (char*)malloc((strlen(temp)+1)*(sizeof(char)));
    strcpy(port,temp);

    temp = strtok(NULL,"/");
    path = (char*)malloc((strlen(temp)+1)*(sizeof(char)));
    strcpy(path,temp);
  }

  //char *msgGet = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
  //char *msgHead =
  //"HEAD /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";

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

  //make socket
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
  serv_addr.sin_addr.s_addr = inet_addr(ipaddr);
  //debug manual ip
  //serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34");


  //connect socket
  if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
    fprintf(stderr, "sockfd connection failed\n");
    return -1;
  }

  //send message
  int msgstatus;
  char *toSend = malloc(strlen(msgHead)*sizeof(char));

  if(isH==1){//select message to send, get or head
    strcpy(toSend,msgHead);
  }
  else{
    strcpy(toSend,msgGet);
  }

  msgstatus = write(sockfd,toSend,strlen(toSend));
  if (msgstatus<0) {
    fprintf(stderr, "message failed to send\n");
  }

  char reply[129];

  if(isH!=1){//read stream to buffer and copy to outfile
    int readcount = read(sockfd,reply,128);
    outfile = fopen("output.dat","w");
    while(readcount==128){
      fprintf(outfile, "%s", reply);
      bzero(reply,128);
      readcount = read(sockfd,reply,128);
    }
    fprintf(outfile, "%s", reply);
  }
  else{//do header and output to stdout
    int readcount = read(sockfd,reply,128);
    while(readcount==128){
      fprintf(stdout, "%s", reply);
      bzero(reply,129);
      readcount = read(sockfd,reply,128);
    }
    fprintf(stdout, "%s", reply);
  }

  //finish up and close everything
  if (isH!=1) {
    fclose(outfile);
  }
  
  shutdown(sockfd,2);
  close(sockfd);

  free(msgHead);
  free(msgGet);
  free(path);
  free(port);
  free(ipaddr);


  return 0;
}
