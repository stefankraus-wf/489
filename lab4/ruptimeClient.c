#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>

int tryconnection(int retries);

int main(int argc, char **argv) {
  int retry = 3;

  tryconnection(retry);

  return 0;
}


int tryconnection(int retries){
  while(retries--){
    printf("Client trying to connect...\n");


  }
  return 0;
}
