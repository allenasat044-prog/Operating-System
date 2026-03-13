#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
sem_t *wrt;
pthread_mutex_t mutex;
int readcount = 0;
void *reader(void *arg)
{
    int id = *(int*)arg;
    pthread_mutex_lock(&mutex);
    readcount++;
    if(readcount == 1)
        sem_wait(wrt);
    pthread_mutex_unlock(&mutex);
    printf("Reader %d is reading\n", id);
    sleep(1);
    pthread_mutex_lock(&mutex);
    readcount--;
    if(readcount == 0)
        sem_post(wrt);
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void *writer(void *arg)
{
    int id = *(int*)arg;
    sem_wait(wrt);
    printf("Writer %d is writing\n", id);
    sleep(1);
    sem_post(wrt);
    return NULL;
}
int main()
{
    pthread_t r[3], w[2];
    int rid[3], wid[2];
    sem_unlink("/wrt");
    wrt = sem_open("/wrt", O_CREAT, 0644, 1);
    if(wrt == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < 3; i++) {
        rid[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &rid[i]);
    }
    for(int i = 0; i < 2; i++) {
        wid[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &wid[i]);
    }
    for(int i = 0; i < 3; i++)
        pthread_join(r[i], NULL);
    for(int i = 0; i < 2; i++)
        pthread_join(w[i], NULL);
    sem_close(wrt);
    sem_unlink("/wrt");
    pthread_mutex_destroy(&mutex);
    return 0;
}
/*
din,write.txt
 for main logic 
 */