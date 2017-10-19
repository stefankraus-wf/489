#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
//30542

void main(int argc, char *argv[]){
  struct sockaddr_in serveraddr, clientaddr;

  int sersock, consock;
  int valread, valwrite;
  int len = sizeof(clientaddr);
  char *sendToYazan = "Can I have a protien bar?";

  char IP_ADDRESS[16] = "192.168.254.13";

  if( (sersock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket() error:\n");
    exit(1);
  }

  serveraddr.sin_family = PF_INET;
  serveraddr.sin_port = htons(22334); //ADD port number
  serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
  //serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(sersock, ((struct sockaddr *) &serveraddr), sizeof(serveraddr));

  listen(sersock, 10);

  char buffer[256];

  memset(buffer, 0, 256);

  while(1){
    consock = accept(sersock, (struct sockaddr *) &clientaddr, &len);
    printf("Connection Accepted\n");
    valread = read(consock, buffer, sizeof(buffer));
    buffer[valread] = '\0';
    printf("%s\n", buffer);
    valwrite = write(consock, sendToYazan, strlen(sendToYazan));
    close(consock);
  }

  close(sersock);
}
