#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 3
int main(int argc, char const *argv[])
{
    int *pid_array = mmap(NULL, SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int pid;

    for (int i = 0; i < SIZE; ++i)
    {
        if ((pid = fork()) < 0)
        {
            perror("fork");
        }
        else if (pid == 0)
        {
            pid_array[i] = getpid();
            exit(0);
        }
        else
        {
            int status;
            if ((pid = waitpid(pid, &status, 0) < 0))
            {
                perror("waitpid");
                munmap(pid_array, SIZE * sizeof(int));
                return -1;
            }
        }
    }

    printf("Child processes IDs in-order:\n");
    for(int i = 0; i < SIZE; ++i)
    {
        printf("[%d] %d\n", i+1, pid_array[i]);
    }

    munmap(pid_array, SIZE * sizeof(int));
    return 0;
}
