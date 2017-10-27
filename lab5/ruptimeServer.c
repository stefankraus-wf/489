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
#include <stdbool.h>

//30542
bool errorCalc(int rate);

void main(int argc, char *argv[]){
  struct sockaddr_in serveraddr, clientaddr, sendAddr;

  int sersock, consock;
  int valread, valwrite;

  int len = sizeof(clientaddr);
  int len2 = sizeof(sendAddr);
  char IP_ADDRESS[16] = "192.168.254.5";
  char sourceAddr1[32], destinationArr1[32];
  int lossRate = 0;

  bool lossPacket = false;

  int i = 0;

  if (argc == 6 ){   // Exactly 3 args: invocation, arg1, arg2
    if (strlen(argv[1]) > 32) {
      //return 1;
    } else {
      //sourceAddr1 = argv[1];
    }

    if (strlen(argv[3]) > 32) {
      //return 1;
    } else {
      //sscanf(argv[2], "%s",  &destinationArr1);
      //destinationArr1 = argv[3];
    }

  } else {
    printf("Error: improper use:\nruptimeClient <Source Address> <Source Port> <Destination Address> <Destination Port> <Loss Rate>\n");
    exit(1);
  }
  lossRate = atoi(argv[5]);


  if( (sersock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket() error:\n");
    exit(1);
  }

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(atoi(argv[2])); //ADD port number
  //serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  sendAddr.sin_family = AF_INET;
  sendAddr.sin_port = htons(atoi(argv[4]));
  sendAddr.sin_addr.s_addr = inet_addr(argv[3]);



  if(bind(sersock, ((struct sockaddr *) &serveraddr), sizeof(serveraddr)) < 0){
    perror("bind() error:\n");
    exit(1);
  }


  //listen(sersock, 10);

  char buffer[2048];
  //char *uptime = "/bin/uptime";

  memset(buffer, 0, 2048);

  while(1){
    //consock = accept(sersock, (struct sockaddr *) &clientaddr, &len);
    //printf("Connection Accepted\n");har *uptime = "/bin/uptime";
    printf("Waiting...\n");
    if( (valread = recvfrom(sersock, buffer, 2048, 0, (struct sockaddr *) &clientaddr, &len))< 0){
      perror("recvfrom() error: \n");
      exit(1);
    }
    buffer[valread] = '\0';
    printf("%d\n",valread);
    printf("Buffer: %s\n", buffer);
    lossPacket = errorCalc(lossRate);
    if(!lossPacket){
      if(sendto(sersock, buffer, valread, 0, (struct sockaddr *) &sendAddr, len2) < 0){
        perror("sendto() error: \n");
        exit(1);
      }
    }

  }

  close(sersock);
}

bool errorCalc(int rate){
    srand(time(NULL));
    int numRand = rand() % (100 + 1 - 0) + 0;
    if(numRand >= rate){
      return false;
    }
    else{
      return true;
    }
}
