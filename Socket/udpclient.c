/* a simple echo client using UDP */
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
#define DEFLEN	64

long delay(struct timeval t1, struct timeval t2);

int main(int argc, char **argv)
{

int data_size = DEFLEN, port = SERVER_UDP_PORT;
int i, j, sd, server_len;
struct hostent *hp;
struct sockaddr_in server;
char *pname, *host, *bp, rbuf[MAXLEN], sbuf[MAXLEN];
struct timeval start, end;

pname = argv[0];
argc--;
argv++;

//Parses the program's input
if (argc > 0 && (strcmp(*argv, "-s") == 0)) {
  if (--argc > 0 && (data_size = atoi(*++argv))) {
    argc--;
    argv++;
  }
  else {
    fprintf(stderr, "Usage: %s [-s data_size] host[port]\n", pname);
    exit(1);
  }
}

//Make whatever argument is left the host port #
if (argc > 0) {
  host = *argv;
  if (--argc > 0) port = atoi(*++argv);
}
else {
  fprintf(stderr, "Usage: %s [-s data_size] host[port]\n", pname);
  exit(1);
}

/************************************************
Socket creation 
SOCK_DGRAM: a connectionles communication port (UDP)
AF_INET: (IPv4) address family
0 = IP
*************************************************/
if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
  fprintf(stderr, "Can't create a socket\n");
  exit(1);
}

/* Store server info */

bzero((char *)&server, sizeof(server)); //zeros out server
server.sin_family = AF_INET; //IPv4
server.sin_port = htons(port); //assigning port #
if ((hp = gethostbyname(host)) == NULL) {
  fprintf(stderr, "Can't get server address\n");
  exit(1);
}
bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);
if (data_size > MAXLEN) {
  fprintf(stderr, "Data is too big\n");
  exit(1);
}

/* Data */
for ( i=0; i < data_size; i++ ) {
  j = (i<26) ? i : i%26;
  sbuf[i] = 'a' + j;
}
gettimeofday(&start, NULL);

/***************************** 
Transmits a message to another socket
 
sendto([socket descriptor], [buffer that has the data],
       [lenth of the data (bytes)], [flags], 
       [address of the target socket],[address size(bytes)])
sd: A descriptor identifying a (possibly connected) socket.
sbuf: A pointer to a buffer containing the data to be transmitted.
data_size: The length, in bytes, of the data pointed to by the buf parameter.
*****************************/
server_len = sizeof(server);
if (sendto(sd, sbuf, data_size, 0, (struct sockaddr *)&server, server_len) == -1) {
  fprintf(stderr, "sendto error\n");
  fprintf(stderr, "Socket Descr. 	== %i\n", sd);
  fprintf(stderr, "Buffer Loc. 	== %i\n", sbuf);
  fprintf(stderr, "data_size 	== %i\n", data_size);
  fprintf(stderr, "socket address	== %i\n", (struct sockaddr *)&server);
  fprintf(stderr, "server_len 	== %i\n", server_len);
  exit(1);
}

/* Receive */
if (recvfrom(sd, rbuf, MAXLEN, 0, (struct sockaddr *)&server, &server_len) < 0) {
  fprintf(stderr, "recvfrom error\n");
  exit(1);
}

gettimeofday(&end, NULL);
printf("Round trip delay: %ld ms.\n", delay(start,end));

if (strncmp(sbuf, rbuf, data_size) != 0) printf("Data is corrupt\n");

/* Clean up and exit */

close(sd);
return(0);

}

/* ----------------------------------------------------------------------------- */

/* Function to compute delay */

long delay (struct timeval t1, struct timeval t2)
{
  long d;
  d = (t2.tv_sec - t1.tv_sec) * 1000;
  d+- ((t2.tv_usec - t1.tv_usec + 500) / 1000);
  return(d);
}
