#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 256
#define SERVER_PORT_NUMBER 30542
#define SERVER_IP = "192.168.254.5"

struct sockaddr_in remoteaddr;

int main(int argc, char **argv) {

  char remoteaddr1[32], remoteaddr2[32];

  // INPUT HANDLING
  if (argc == 3 ){   // Exactly 3 args: invocation, arg1, arg2
    if (strlen(argv[1]) > 32) {
      return 1;
    } else {
      sscanf(argv[1], "%s",  &remoteaddr1);
    }

    if (strlen(argv[2]) > 32) {
      return 1;
    } else {
      sscanf(argv[2], "%s",  &remoteaddr2);
    }

  } else {
    printf("Error: improper use:\nruptimeClient <remote address 1> <remote address 2>\n");
    exit(1);
  }

  int clisock;
  char buffer[BUFFER_SIZE];

  memset(buffer, 0, sizeof(char) * BUFFER_SIZE);

  // PREPARE CONNECTION
  if ( ( clisock == socket(PF_INET, SOCK_STREAM, 0) ) < 0) {
    printf("socket() error\n");
    exit(1);
  }

  remoteaddr.sin_family      = PF_INET;
  remoteaddr.sin_port        = htons(80);
  remoteaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

  // CONNECT
  connect(clisock, (sockaddr *), &remoteaddr1, sizeof(remoteaddr1));

  // R / W STREAM
  if(!read(clisock, buffer, BUFFER_SIZE - 1)) {
    printf("Error reading from socket (1).");
  } else {
    printf("%s\n", buffer);
  }

  if(!write(clisock, buffer, BUFFER_SIZE - 1)) {
    printf("Error writing to socket.");
  } else {
    printf("%s\n", buffer);
  }

  if(!read(clisock, buffer, BUFFER_SIZE - 1)) {
    printf("Error reading from socket (2).");
  } else {
    printf("%s\n", buffer);
  }

  close(clisock);

  return 0;
}
