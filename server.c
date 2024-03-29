// Largely inspired by https://bruinsslot.jp/post/simple-http-webserver-in-c/
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 2048

int main() {

  char buffer[BUFFER_SIZE];
  char resp[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html>hello, world</html>\r\n";
  // create socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("webserver (socket)");
    return 1;
  }
  printf("socket created successfully\n");

  // create addr to bind socket to
  struct sockaddr_in host_addr;

  int host_addrlen = sizeof(host_addr);

  host_addr.sin_family = AF_INET;
  host_addr.sin_port = htons(PORT);
  host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // bind socket to the addr
  if (bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0) {
    perror("webserver (bind)");
    return 1;
  }

  printf("socket successfully bound to address\n");

  // listen for incoming connections
  if (listen(sockfd, SOMAXCONN) != 0) {
    perror("webserver listen");
    return 1;


  }
  printf("server listening for connections\n");

  while (1) {
    // accept incoming connections
    int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr, (socklen_t *)&host_addrlen);
    if (newsockfd < 0) {
        perror("webserver accept");
        continue;
    }
    printf("connection accepted\n");

    // read from the socket
    int valread = read(newsockfd, buffer, BUFFER_SIZE);
    if (valread < 0) {
       perror("webserver read");
       continue;
    }

    // write to socket
    int valwrite = write(newsockfd, resp, strlen(resp));
    if (valwrite < 0) {
      perror("webserver write");
      continue;
    }
    close(newsockfd);
  }
  return 0;
 }

