#include <sys/types.h>
#include <sys/socket.h>
#include <stlib.h>
#include <stdio.h>

struct sockaddr_in serveraddr, clientaddr;

int sersock, consock;
int len = sizeof(clientaddr);

char IP_ADDRESS = "";

if( (sersock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
  perror("socket() error:\n");
  exit(1);
}

serveraddr.sin_family = PR_INET;
serveraddr.sin_port = htons(); //ADD port number
serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
//serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

bind(sersock, (sockaddr *) &serveraddr, sizeof(serveraddr));
listen(sersock, 10);

while(1){
  consock = accept(sersock, (sockaddr *) &clientaddr, &len);

  //read
  //write
  close(consock);
}

close(sersock);
