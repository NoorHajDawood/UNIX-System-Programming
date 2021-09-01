#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static bool *chopsticks = NULL;
static int n = 0;

bool liftChopstick(int chopstick)
{
    pthread_mutex_lock(&mutex);
    if (chopsticks[chopstick] == false)
    {
        chopsticks[chopstick] = true;
        pthread_mutex_unlock(&mutex);
        return true;
    }
    pthread_mutex_unlock(&mutex);
    return false;
}

void eat(int philos_num, int chopstick1, int chopstick2)
{
    int r = rand() % 10 + 1;
    printf("Philosopher %d is eating for %d seconds\n", philos_num, r);
    sleep(r);
    printf("Philosopher %d is done eating for %d seconds\n", philos_num, r);

    pthread_mutex_lock(&mutex);
    printf("Philosopher %d released [%d] [%d]\n", philos_num, chopstick1 + 1, chopstick2 + 1);
    chopsticks[chopstick1] = false;
    chopsticks[chopstick2] = false;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void think(int philos_num)
{
    int r = rand() % 11 + 10;
    printf("Philosopher %d is thinking for %d seconds\n", philos_num, r);
    sleep(r);
    printf("Philosopher %d is done thinking for %d seconds\n", philos_num, r);
}

void *dine(void *id)
{
    bool flag = false;
    int philos_num = (int)((long)id);
    int chopstick1 = philos_num - 1, chopstick2 = philos_num;

    if (philos_num == n)
    {
        chopstick1 = 0;
        chopstick2 = n - 1;
    }

    while (1)
    {
        while (!flag)
        {
            printf("Philosopher %d is trying to lift the 1st chopstick\n", philos_num);
            flag = liftChopstick(chopstick1);
            if (!flag)
            {
                pthread_cond_wait(&cond, &mutex);
                pthread_mutex_unlock(&mutex);
            }
        }
        printf("Philosopher %d succefully lifted the 1st chopstick [%d]\n", philos_num, chopstick1 + 1);

        while (!flag)
        {
            printf("Philosopher %d is trying to lift the 2nd chopstick\n", philos_num);
            flag = liftChopstick(chopstick2);
            if (!flag)
            {
                pthread_cond_wait(&cond, &mutex);
                pthread_mutex_unlock(&mutex);
            }
        }
        printf("Philosopher %d succefully lifted the 2nd chopstick [%d]\n", philos_num, chopstick2 + 1);

        eat(philos_num, chopstick1, chopstick2);

        think(philos_num);
    }

    return NULL;
}

int main(int argc, const char *argv[])
{
    srand(time(NULL));

    if (argc != 2)
    {
        fprintf(stderr, "Expected a number in the command line.\n");
        return 1;
    }
    
    n = atoi(argv[1]);
    if (n <= 0)
    {
        fprintf(stderr, "Expected a positive number.\n");
        return 2;
    }

    pthread_t *threadArray = (pthread_t *)malloc(n * sizeof(pthread_t));
    chopsticks = (bool *)malloc(n * sizeof(bool));

    for (int i = 0; i < n; ++i)
    {
        chopsticks[i] = false;
        if (pthread_create(&threadArray[i], NULL, dine, (void *)((long)i + 1)))
        {
            perror("pthread_create");
            return 4;
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (pthread_join(threadArray[i], NULL))
        {
            perror("pthread_join");
            return 4;
        }
    }

    return 0;
}
