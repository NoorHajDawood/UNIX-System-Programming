#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_LINE_SIZE 2048
int main(int argc, const char *argv[])
{
    char line[MAX_LINE_SIZE];
    pid_t pid;
    printf("> ");
    while (fgets(line, MAX_LINE_SIZE, stdin) != NULL)
    {
        line[strlen(line) - 1] = 0;
        if ((pid = fork()) < 0)
            perror("fork");
        else if (!pid)
        {
            char *args[] = {line, NULL};
            if (execve(line, args, NULL) < 0)
            {
                perror("execve");
                exit(1);
            }
        }
        else
        {
            int status;
            if ((pid = waitpid(pid, &status, 0) < 0))
                perror("waitpid");
        }
        printf("> ");
    }
    exit(0);
}