#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
#define PORT 1337


int main(int argc, char **argv) {
  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int port_number; /* port to listen on */
  unsigned int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  /*
   * check command line arguments
   */
  if (argc == 2) {
    port_number = atoi(argv[1]);
  } else { // fall back to default
    port_number = PORT;
  }

  /*
   * socket: create the parent socket
   */
  parentfd = socket(PF_INET, SOCK_STREAM, 0);
  if (parentfd < 0){
    perror("ERROR opening socket");
    exit(1);
  }

  /* setsockopt: Handy debugging trick that lets
   * us rerun the server immediately after we kill it;
   * otherwise we have to wait about 20 secs.
   * Eliminates "ERROR on binding: Address already in use" error.
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  memset((char *) &serveraddr, '\0', sizeof(serveraddr));

  serveraddr.sin_family = PF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)port_number);

  if (bind(parentfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
    perror("ERROR on binding");
    exit(1);
  }

  /*
   * listen: make this socket ready to accept connection requests
   */
  if (listen(parentfd, 5) < 0){ /* allow 5 requests to queue up */
    perror("ERROR on listen");
    exit(1);
  }

  printf("Server Listening on port %d\n", port_number);

  clientlen = sizeof(clientaddr);


  while (1) { // loop forever

    /*
     * accept: wait for a connection request
     */
    childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (childfd < 0){
      perror("ERROR on accept");
      exit(1);
    }

    /*
     * gethostbyaddr: determine who sent the message (debugging only)
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL){
      perror("ERROR on gethostbyaddr");
      exit(1);
    }
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL){
      perror("ERROR on inet_ntoa\n");
      exit(1);
    }
    printf("server connected with %s (%s)\n", hostp->h_name, hostaddrp);

    /*
    * get uptime of server
    */
    char uptime[1024];
    FILE *fp = popen("uptime", "r");

    fscanf(fp, "%[^\n]%*c", &uptime);
    pclose(fp);

    /*
     * write the uptime to the socket
     */
    n = write(childfd, uptime, strlen(uptime));
    if (n < 0){
      perror("ERROR writing to socket");
      exit(1);
    }

    /*
    * close port
    */
    close(childfd);
  }
}
