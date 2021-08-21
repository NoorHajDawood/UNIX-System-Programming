#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _array
{
    int length;
    int *array;
} array;

void *findMin(void *arg)
{
    array arr = *((array *)arg);
    if (arr.length <= 0)
        return (void *)0;
    long min = arr.array[0];
    for (int i = 1; i < arr.length; ++i)
        if (min > arr.array[i])
            min = arr.array[i];
    return (void *)min;
}

int main(int argc, const char *argv[])
{
    pthread_t tid1, tid2;
    void *ret;
    long min;
    int length = argc - 1;
    int *arr;
    array arr1 = {length / 2 + length % 2, arr};
    array arr2 = {length - length / 2 - length % 2, arr + length / 2 + length % 2};


    if (argc <= 1)
    {
        fprintf(stderr, "expected array values\n");
        return 1;
    }

    arr = (int *)malloc(length * sizeof(int));
    if(arr == NULL)
    {
        perror("malloc");
        return 2;
    }
    
    for (int i = 1; i < argc; i++)
    {
        arr[i - 1] = atol(argv[i]);
    }

    if (pthread_create(&tid1, NULL, findMin, &arr1))
    {
        perror("pthread_create(1)");
        return 1;
    }

    if (argc > 2)
        if (pthread_create(&tid2, NULL, findMin, &arr2))
        {
            perror("pthread_create(2)");
            return 1;
        }

    if (pthread_join(tid1, &ret))
    {
        fprintf(stderr, "Could not join tid1 Thread\n");
        return 1;
    }

    min = (long)ret;

    if (argc > 2)
    {
        if (pthread_join(tid2, &ret))
        {
            fprintf(stderr, "Could not join with tid2 Thread\n");
            return 1;
        }

        min = min < (long)ret ? min : (long)ret;
    }

    printf("Min = [%ld]\n", min);

    return 0;
}
