/* a simple echo server using UDP */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <sys/time.h>

#define SERVER_UDP_PORT	5000
#define MAXLEN	4096

int main(int argc, char **argv)
{

int n, sd, client_len, port;
struct sockaddr_in server, client;
char buf[MAXLEN];

switch(argc) {
case 1:
	port = SERVER_UDP_PORT;
	break;
case 2:
	port = atoi(argv[1]);
	break;
default:
	fprintf(stderr, "Usage: %s [port]\n", argv[0]);
	exit(1);
}

/* Create a dgram socket */

if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
  fprintf(stderr, "Can't create a socket\n");
  exit(1);
}

/* Bind an address to the socket */

memset(&server, 0, sizeof(server));
server.sin_family = AF_INET;
server.sin_port = htons(port);
server.sin_addr.s_addr = htonl(INADDR_ANY);
if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
  fprintf(stderr, "Can't bind name to socket\n");
  exit(1);
}

/* Connect */

while(1) {
  client_len = sizeof(client);
  if ((n = recvfrom(sd, buf, MAXLEN, 0, (struct sockaddr *)&client, &client_len)) < 0) {
    fprintf(stderr, "Can't receive datagram\n");
    exit(1);
  }

  printf("Received [%s] (%d bytes) from %s\n", buf,(int)strlen(buf),(char *)inet_ntoa(client.sin_addr));
  
  if (sendto(sd, buf, n, 0, (struct sockaddr *)&client, client_len) != n) {
    fprintf(stderr, "Can't send datagram\n");
    exit(1);
  }

  // Clear out the buffer
  memset(&buf, 0, MAXLEN);
}

/* Clean up and exit */

close(sd);
return(0);

}
