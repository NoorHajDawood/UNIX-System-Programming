#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define SIZE 1024

int main(int argc, char const *argv[])
{
    int fd, n;
    char buf[SIZE];
    char str[] = "abcdefghjklmnopqrstu";
    if ((fd = open("AFile", O_WRONLY | O_CREAT, 0666)) < 0)
        perror("open write");
    if ((n = write(fd, str, sizeof(str))) < 0)
        perror("write");
    if(close(fd) < 0)
        perror("close");

    if ((fd = open("AFile", O_RDONLY)) < 0)
        perror("open read");
    if ((n = read(fd, buf, SIZE)) < 0)
        perror("read");
    printf("Amount of bytes read = %d\n", n);

    if ((n = read(fd, buf, SIZE)) < 0)
        perror("read");
    printf("Amount of bytes read = %d\n", n);
    
    if(close(fd) < 0)
        perror("close");
    return 0;
}
