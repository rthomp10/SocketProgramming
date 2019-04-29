/* 
 * Echo server using TCP
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_TCP_PORT	3000
#define BUFLEN	256

int main(int argc, char **argv)
{

  int i, n, bytes_to_read;
  int sd, new_sd, client_len, port, hostname;
  struct sockaddr_in server, client;
  char *bp, buf[BUFLEN], tbuf[BUFLEN]="hello";
  char hostbuf[256], *IPbuf;
  struct hostent *host;
  
  switch(argc) {
  case 1:
    port = SERVER_TCP_PORT;
    break;
  case 2:
    port = atoi(argv[1]);
    break;
  default:
    fprintf(stderr, "Usage: %s [port]\n", argv[0]);
    exit(1);
  }
  
  /* Create a stream socket */
  
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "ERROR: Can't create a socket\n");
    exit(1);
  }
  
  /* Bind an address to the socket */
  
  bzero((char *)&server, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    fprintf(stderr, "ERROR: Can't bind name to socket\n");
    exit(1);
  }

 /* Get host name, etc. from socket structure */

  if(gethostname(hostbuf, sizeof(hostbuf)) == -1) {
    fprintf(stderr, "ERROR: Problem with hostname\n");
    exit(1);
  }
  host = gethostbyname(hostbuf);
  if (host==NULL) {
   fprintf(stderr, "ERROR: Problem with hostname\n");
    exit(1);
  }
  IPbuf = inet_ntoa(*((struct in_addr*) host->h_addr_list[0])); 
  printf("\n");
  printf("Server listening ... \n");
  printf("\t host: %s\n", hostbuf); 
  printf("\t IP: %s\n", IPbuf);
  printf("\t port: %d\n\n", port);

  /* Queue connect requests */
  
  listen(sd,5);
  
  client_len = sizeof(client);
  if ((new_sd = accept(sd, (struct sockaddr *)&client, &client_len)) == -1) {
    fprintf(stderr, "ERROR: Can't accept client\n");
    exit(1);
  }
  
  printf("Got connection from client: %s\n", (char *)inet_ntoa(client.sin_addr));
  
  printf("Reading data from socket ...\n");
  bzero(buf,BUFLEN);
  n = read(new_sd,buf,BUFLEN-1);
  if (n<0) {
    fprintf(stderr, "ERROR: Problem reading from socket");
    exit(1);
  }

  printf(".. received [%s] (%d bytes) from client\n", buf,(int)strlen(buf));
  
  n = write(new_sd, buf, strlen(buf));
  if (n<0) {
    fprintf(stderr, "ERROR: Problem writing to socket");
    exit(1);
  }
  
  /* Clean up and exit */
  
  close(new_sd);
  close(sd);
  return(0);
}

