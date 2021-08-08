#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
static int limit = 0;
static void my_sig_int(int sig_number)
{
    static int counter = 0;
    printf("%d Interrupt, Hahaha\n", ++counter);
    --limit;
    
}
int main(int argc, const char *argv[])
{
    if(argc < 2)
        limit = 1;
    else
        limit = atoi(argv[1]);
    if (signal(SIGINT, my_sig_int) == SIG_ERR)
        perror("signal");
    while(limit>0);
    return 0;
}