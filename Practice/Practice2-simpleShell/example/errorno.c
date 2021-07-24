#include <stdio.h>
#include <string.h>
#include <errno.h>
int main(int argh, const char *argv[])
{
    printf("errno: %d\n", errno);
    if (execve(NULL, NULL, NULL) < 0)
    {
        printf("errno: %d\n", errno);
        printf("strerror: %s\n", strerror(errno));
        perror("execute");
    }
    return 0;
}