#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE  1024
// default hostname, port
#define HOSTNAME "127.0.0.1"
#define PORT     1337

int main(int argc, char **argv) {
    int sockfd, port_number, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];

    /* check command line arguments */
    if (argc == 3) {
      hostname = argv[1];
      port_number = atoi(argv[2]);
    } else { // fall back to defaults
      hostname = HOSTNAME;
      port_number = PORT;
    }

    /* create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("ERROR opening socket");
        exit(1);
      }

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(1);
    }

    /* build the server's Internet address */
    memset((char *) &serveraddr, '\0', sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    strcpy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(port_number);

    /* connect: create a connection with the server */
    if (connect(sockfd, &serveraddr, sizeof(serveraddr)) < 0){
      perror("ERROR connecting");
      exit(1);
    }

    /* print the server's reply */
    memset(buf, '\0', BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0){
      perror("ERROR reading from socket");
      exit(1);
    }
    printf("%s\n", buf);
    close(sockfd);
    return 0;
}
