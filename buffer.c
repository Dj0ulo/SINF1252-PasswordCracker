#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "main.h"
#include "buffer.h"

size_t BUFFER_SIZE;
uint8_t **buffer;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void initBuffer(const size_t size)
{
    BUFFER_SIZE = size;
    buffer = (uint8_t **)malloc(BUFFER_SIZE * sizeof(uint8_t*));
    if(buffer)
        for(int i=0;i<BUFFER_SIZE;i++)
            buffer[i] = NULL;
    else
        logi("Error malloc", "initBuffer");


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
int isBufferEmpty()
{
    for(int i=0;i<BUFFER_SIZE;i++)
        if(buffer[i])
            return 0;
    return 1;
}
void insertInBuffer(uint8_t *hash)
{
    if(!hash)
        return;
    sem_wait(&empty); // attente d'un slot libre
    pthread_mutex_lock(&mutex);
    // section critique
    for(int i=0;i<BUFFER_SIZE;i++)
        if(buffer[i] == NULL)
        {
            buffer[i] = (uint8_t *)malloc(HASH_SIZE);
            if(!buffer[i])
            {
                logi("Error malloc", "insertInBuffer");
                break;
            }
            memcpy(buffer[i], hash, HASH_SIZE);
            break;
        }
    pthread_mutex_unlock(&mutex);
    sem_post(&full); // il y a un slot rempli en plus
}
uint8_t *removeFromBuffer()
{
    sem_wait(&full); // attente d'un slot rempli
    pthread_mutex_lock(&mutex);
    // section critique
    uint8_t *r = NULL;
    for(int i=0;i<BUFFER_SIZE;i++)
        if(buffer[i])
        {
            r = (uint8_t *)malloc(HASH_SIZE);
            if(!r){
                logi("Error malloc", "removeFromBuffer");
                break;
            }
            memcpy(r, buffer[i], HASH_SIZE);
            free(buffer[i]);
            buffer[i] = NULL;
            break;
        }
    pthread_mutex_unlock(&mutex);
    sem_post(&empty); // il y a un slot libre en plus
    return r;
}

///Pour débuguer------------------------------------------
extern pthread_mutex_t mtxPrt;
void printHash(const char *one,const uint8_t *buff)
{
    pthread_mutex_lock(&mtxPrt);
    printf("%s : ",one);
    for(int i=0;i<HASH_SIZE;i++)
        printf("|%x",buff[i]);
    printf("\n");
    pthread_mutex_unlock(&mtxPrt);
}
