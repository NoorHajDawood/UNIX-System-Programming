#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t my_thread;
void print_my_ids(void)
{
  pid_t pid;
  pthread_t tid;
  pid = getpid();
  tid = pthread_self();
  printf("print_my_ids: My PID = %u,My TID = %u\n", pid, (unsigned)tid);
}

void* foo (void* arg)
{
  printf("Thread:%u\n", my_thread);
  print_my_ids();
  printf("Thread:%u Again!\n", my_thread);
  return NULL;
}

int main (int argc, const char* argv[])
{
  if (pthread_create(&my_thread, NULL, foo, NULL))
    return 1;

  printf("Main Thread:");
  print_my_ids();
  printf("Main Thread Again!");


  sleep(2);

  return 0;
}
