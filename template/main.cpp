#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>


char response[] = "HTTP/1.1 200 OK\r\n" 
"Content-Length: 27\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<html>hello world</html>\r\n";

int main()
{
  int one = 1, client_fd;
  struct sockaddr_in svr_addr, cli_addr;
  socklen_t sin_len = sizeof(cli_addr);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    err(1, "can't open socket");

  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

  int port = 8080;
  svr_addr.sin_family = AF_INET;
  if (getenv("OPENSHIFT_CPP_IP") != NULL) {
  	  svr_addr.sin_addr.s_addr = inet_addr(getenv("OPENSHIFT_CPP_IP"));
  } else {
  	  svr_addr.sin_addr.s_addr = INADDR_ANY;
	  printf("Using 0.0.0.0\n");
  }

  svr_addr.sin_port = htons(port);

  if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
    close(sock);
    err(1, "Can't bind");
  }

  listen(sock, 5);
  while (1) {
    client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
    printf("got connection\n");

    if (client_fd == -1) {
      perror("Can't accept");
      continue;
    }

    write(client_fd, response, sizeof(response) - 1); /*-1:'\0'*/
    close(client_fd);
  }
}
