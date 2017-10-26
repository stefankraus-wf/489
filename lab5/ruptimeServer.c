#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

//30542

void main(int argc, char *argv[]){
  struct sockaddr_in serveraddr, clientaddr;

  //int sysinfo(struct sysinfo *info);

  int sersock, consock;
  int valread, valwrite;


  int len = sizeof(clientaddr);

  char IP_ADDRESS[16] = "192.168.254.8";
  int logsize = 10;
  int i = 0;



  if( (sersock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket() error:\n");
    exit(1);
  }

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(30542); //ADD port number
  //serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(sersock, ((struct sockaddr *) &serveraddr), sizeof(serveraddr)) < 0){
    perror("bind() error:\n");
    exit(1);
  }


  //listen(sersock, 10);

  char buffer[256];
  //char *uptime = "/bin/uptime";

  memset(buffer, 0, 256);

  while(1){
    //consock = accept(sersock, (struct sockaddr *) &clientaddr, &len);
    //printf("Connection Accepted\n");har *uptime = "/bin/uptime";
    printf("Waiting...\n");
    if( valread = recvfrom(sersock, buffer, 256, 0, (struct sockaddr *) &clientaddr, len) < 0){
      perror("recvfrom() error: \n");
      exit(1);
    }
    buffer[valread] = '\0';
    printf("%s\n", buffer);
    if(sendto(sersock, buffer, valread, 0, (struct sockaddr *) &clientaddr, len) < 0){
      perror("sendto() error: \n");
      exit(1);
    }
  }

  close(sersock);
  return 0;
}
