#include <stdio.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    struct dirent *pDirent;
    DIR *pDir;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./my_ls <dirname>\n");
        return 1;
    }

    pDir = opendir(argv[1]);
    if (pDir == NULL)
    {
        fprintf(stderr, "Cannot open directory '%s'\n", argv[1]);
        return 2;
    }

    while ((pDirent = readdir(pDir)) != NULL)
    {
        printf("[%s]\n", pDirent->d_name);
    }
    if(errno != 0)
    {
        perror("readdir");
        return 3;
    }
    closedir(pDir);
    return 0;
}
