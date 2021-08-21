#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define BUF_LEN 256
#define PORT 8080

void communicate(int sockfd)
{
    char buff[BUF_LEN];
    int n;
    bzero(buff, BUF_LEN);

    n = read(sockfd, buff, BUF_LEN);
    if(n<0)
        perror("read");
    printf("From client: %s\n", buff);
    
    strcpy(buff, "Hey There!");
    n = write(sockfd, buff, BUF_LEN);
    if(n<0)
        perror("write");
}

int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		perror("bind\n");
		exit(0);
	}

	if ((listen(sockfd, 5)) != 0) {
		perror("listen");
		exit(0);
	}
	else
		printf("Server listening..\n");
	
    len = sizeof(cli);
	connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
	if (connfd < 0) {
		perror("accept");
		exit(0);
	}
	else
		printf("server acccepted the client\n");

    communicate(connfd);
    close(connfd);
	close(sockfd);
}
