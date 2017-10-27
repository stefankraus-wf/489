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
  //Takes loss rate from command line
  lossRate = atoi(argv[5]);


  if( (sersock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket() error:\n");
    exit(1);
  }

  serveraddr.sin_family = AF_INET;
  //Port from command line
  serveraddr.sin_port = htons(atoi(argv[2]));
  //serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  //Address to send to from command line
  sendAddr.sin_family = AF_INET;
  sendAddr.sin_port = htons(atoi(argv[4])); //port from command line
  sendAddr.sin_addr.s_addr = inet_addr(argv[3]); //IP from command line



  if(bind(sersock, ((struct sockaddr *) &serveraddr), sizeof(serveraddr)) < 0){
    perror("bind() error:\n");
    exit(1);
  }




  char buffer[2048];


  memset(buffer, 0, 2048);

  while(1){
    //Waits until received packets & writes to buffer
    printf("Waiting...\n");
    if( (valread = recvfrom(sersock, buffer, 2048, 0, (struct sockaddr *) &clientaddr, &len))< 0){
      perror("recvfrom() error: \n");
      exit(1);
    }

    buffer[valread] = '\0';
    printf("%d\n",valread);
    printf("Buffer: %s\n", buffer);
    //Calculates error if any, & drops packet if within the error margin
    lossPacket = errorCalc(lossRate);

    //Sends packet to sendAddr if no error for that packet
    if(!lossPacket){
      if(sendto(sersock, buffer, valread, 0, (struct sockaddr *) &sendAddr, len2) < 0){
        perror("sendto() error: \n");
        exit(1);
      }
    }

  }

  close(sersock);
}

//Calculates error by getting random number 0-100 and comparing to error rate from the command line
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
