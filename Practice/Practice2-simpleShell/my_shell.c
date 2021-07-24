#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char cwd[256];
    char buffer[256] = {0};
    char *token = 0;
    char *space = " \t\n";
    const char *workDir;
    struct dirent *pDirent;
    DIR *pDir = NULL;
    FILE *fptr = NULL;

    if (argc != 2)
        workDir = "./";
    else
        workDir = argv[1];
    if (chdir(workDir) != 0)
        perror("chdir() error()");
    else
    {
        if (getcwd(cwd, sizeof(cwd)) == NULL)
            perror("getcwd() error");
        else
            printf("Current working directory is: %s\n", cwd);
    }

    while (1)
    {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        token = strtok(buffer, space);
        if (!token)
            continue;
        if (!strcmp(token, "ls"))
        {
            token = strtok(NULL, space);
            char *currentDir = NULL;
            if (!token || !strcmp(token, ">"))
                currentDir = cwd;
            else
                currentDir = token;
            pDir = opendir(currentDir);
            if (pDir == NULL)
            {
                fprintf(stderr, "Cannot open directory '%s'\n", currentDir);
                continue;
            }

            if (token && strcmp(token, ">"))
                token = strtok(NULL, space);
            if (token && !strcmp(token, ">"))
            {
                token = strtok(NULL, space);
                fptr = fopen(token, "w");
                if (fptr == NULL)
                {
                    fprintf(stderr, "Cannot open file for write: '%s'", token);
                    closedir(pDir);
                    continue;
                }
            }
            if (fptr == NULL)
                fptr = stdout;
            while ((pDirent = readdir(pDir)) != NULL)
            {
                fprintf(fptr, "[%s]\n", pDirent->d_name);
            }
            if (errno != 0)
            {
                perror("readdir");
                return 3;
            }
            closedir(pDir);
            if (fptr != stdout)
                fclose(fptr);
        }
        // else if (!strcmp(token, "cd"))
        // {
        //     token = strtok(NULL, " \t\n");
        //     if(!token || !strlen(token))
        //     {
        //         fprintf(stderr, "Command 'cd' is expecting an argument\n");
        //     }
        // }
        else
        {
            fprintf(stderr, "Unknown command: '%s'\n", token);
        }
    }
    return 0;
}
