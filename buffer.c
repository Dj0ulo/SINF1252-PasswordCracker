#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "buffer.h"

char **buffer;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void initBuffer()
{
    buffer = malloc(BUFFER_SIZE * sizeof(char*));

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0 , BUFFER_SIZE); // buffer vide
    sem_init(&full, 0 , 0); // buffer vide
}
void freeBuffer()
{
    if(buffer==NULL)
        return;
    for(int i=0;i<BUFFER_SIZE;i++)
        if(buffer[i])
            free(buffer[i]);
    free(buffer);
}

void insertInBuffer(char *hash)
{
    sem_wait(&empty); // attente d'un slot libre
    pthread_mutex_lock(&mutex);

    // section critique
    for(int i=0;i<BUFFER_SIZE;i++)
        if(!buffer[i])
        {
            buffer[i] = malloc(HASH_SIZE);
            memcpy(buffer[i], hash, HASH_SIZE);
        }

    pthread_mutex_unlock(&mutex);
    sem_post(&full); // il y a un slot rempli en plus
}
char *removeFromBuffer()
{
    sem_wait(&full); // attente d'un slot rempli
    pthread_mutex_lock(&mutex);

    // section critique
    char *r = NULL;
    for(int i=0;i<BUFFER_SIZE;i++)
        if(buffer[i])
        {
            r = malloc(HASH_SIZE);
            memcpy(r, buffer[i], HASH_SIZE);
            free(buffer[i]);
            break;
        }

    pthread_mutex_unlock(&mutex);
    sem_post(&empty); // il y a un slot libre en plus
    return r;
}
