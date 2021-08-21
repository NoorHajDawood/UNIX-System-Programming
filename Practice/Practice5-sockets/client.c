#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_LEN 80
#define PORT 8080

void communicate(int sockfd)
{
    char buff[BUF_LEN];
    int n;
    bzero(buff, BUF_LEN);
    
    strcpy(buff, "Hey!");
    n = write(sockfd, buff, BUF_LEN);
    if(n<0)
        perror("write");

    n = read(sockfd, buff, BUF_LEN);
    if(n<0)
        perror("read");
    printf("From server: %s\n", buff);

}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;//AF_UNIX
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
		perror("connect");
		exit(0);
	}
	else
		printf("connected to the server\n");

    communicate(sockfd);
	close(sockfd);
}
