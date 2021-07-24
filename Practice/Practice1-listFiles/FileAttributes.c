#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    struct stat file_stats;
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./FileAttributes 'file-path'\n");
        return 1;
    }

    if(stat(argv[1], &file_stats) < 0)
    {
        perror("stat");
        return 2;
    }

    printf("std_ino: %ld\n", file_stats.st_ino);
    printf("st_uid: %ud\n", file_stats.st_uid);
    printf("st_gid: %ud\n", file_stats.st_gid);
    printf("st_atime: %ld\n", file_stats.st_atime);
    printf("st_mtime: %ld\n", file_stats.st_mtime);
    printf("st_ctime: %ld\n", file_stats.st_ctime);
    printf("st_size: %ld\n", file_stats.st_size);
    
    return 0;
}
