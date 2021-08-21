#include <stdio.h>
#include <stdlib.h>
extern char **environ;
int main(int argc, const char *argv[])
{
    int i;
    for (i = 0; environ[i] != NULL; ++i)
        printf("%s\n", environ[i]);
    printf("HOME:%s\n", getenv("HOME"));
    return 0;
}