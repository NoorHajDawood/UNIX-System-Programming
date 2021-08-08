#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int calcRemaining(int x)
{
    int sum = 0;
    for (int i = 1; i < x; i++)
    {
        sum += i;
    }
    return sum;
}

int calcOffset(int x, int n)
{
    int sum = 0;
    for(; n <= x; ++n)
    {
        sum += n/2+n%2;
    }
    return sum;
}

int main(int argc, char const *argv[])
{
    char x;
    scanf("%c", &x);
    char n = x;
    int out = open("my_exercise.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0)
    {
        perror("open my_exercise");
        exit(1);
    }
    
    for(int i; n > '0'; --n)
    {
        for (i = (n-'0'); i > (n-'0')/2; --i)
        {
            write(out, &n, 1);
        }
        lseek(out, calcRemaining(n-'0'), SEEK_CUR);
        for (; i > 0; --i)
        {
            write(out, &n, 1);
        }
        lseek(out, calcOffset(x-'0', n-'0'), SEEK_SET);
    }

    close(out);
    return 0;
}
