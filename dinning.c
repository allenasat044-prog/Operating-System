#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#define N 5
sem_t *chopstick[N];
void *philosopher(void *num)
{
    int id = *(int*)num;
    printf("Philosopher %d is thinking\n", id);
    sleep(1);
    if(id == N - 1) {
        sem_wait(chopstick[(id + 1) % N]);
        sem_wait(chopstick[id]);
    } else {
        sem_wait(chopstick[id]);
        sem_wait(chopstick[(id + 1) % N]);
    }
    printf("Philosopher %d is eating\n", id);
    sleep(2);
    sem_post(chopstick[id]);
    sem_post(chopstick[(id + 1) % N]);
    printf("Philosopher %d finished eating\n", id);
    return NULL;
}
int main()
{
    pthread_t ph[N];
    int id[N];
    char sem_name[20];
    for(int i = 0; i < N; i++) {
        snprintf(sem_name, sizeof(sem_name), "/chopstick%d", i);
        sem_unlink(sem_name);
        chopstick[i] = sem_open(sem_name, O_CREAT, 0644, 1);
        if(chopstick[i] == SEM_FAILED) {
            perror("sem_open failed");
            return 1;
        }
    }
    for(int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&ph[i], NULL, philosopher, &id[i]);
    }
    for(int i = 0; i < N; i++)
        pthread_join(ph[i], NULL);
    for(int i = 0; i < N; i++) {
        snprintf(sem_name, sizeof(sem_name), "/chopstick%d", i);
        sem_close(chopstick[i]);
        sem_unlink(sem_name);
    }
    return 0;
}
/*
din,write.txt
 for main logic 
 */