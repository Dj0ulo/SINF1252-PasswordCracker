#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "constants.h"
#include "files.h"
#include "bufferRes.h"

size_t BUFFER_RES_SIZE;
char **bufferRes;
pthread_mutex_t mutexRes;
sem_t semResEmpty;
sem_t semResfull;

void initBufferRes(const size_t size)
{
    BUFFER_RES_SIZE = size;
    bufferRes = (char **)malloc(BUFFER_RES_SIZE * sizeof(uint8_t*));
    if(bufferRes)
        for(size_t i=0;i<BUFFER_RES_SIZE;i++)
            bufferRes[i] = NULL;
    else
        logi("Error malloc", "initBufferRes");


    pthread_mutex_init(&mutexRes, NULL);
    sem_init(&semResEmpty, 0 , BUFFER_RES_SIZE); // buffer vide
    sem_init(&semResfull, 0 , 0); // buffer vide
}
void freeBufferRes()
{
    if(bufferRes==NULL)
        return;
    for(size_t i=0;i<BUFFER_RES_SIZE;i++)
        if(bufferRes[i])
            free(bufferRes[i]);
    free(bufferRes);
}
int isBufferResEmpty()
{
    for(size_t i=0;i<BUFFER_RES_SIZE;i++)
        if(bufferRes[i])
            return 0;
    return 1;
}
void insertInBufferRes(char *pswd)
{
    if(!pswd)
        return;
    sem_wait(&semResEmpty); // attente d'un slot libre
    pthread_mutex_lock(&mutexRes);
    // section critique
    for(size_t i=0;i<BUFFER_RES_SIZE;i++)
        if(bufferRes[i] == NULL)
        {
            bufferRes[i] = (char *)malloc(MAX_SIZE_PSWD + 1);
            if(!bufferRes[i])
            {
                logi("Error malloc", "insertInBufferRes");
                break;
            }
            strcpy(bufferRes[i], pswd);
            break;
        }
    pthread_mutex_unlock(&mutexRes);
    sem_post(&semResfull); // il y a un slot rempli en plus
}
int removeFromBufferRes(char * pswd)
{
    int r = 0;
    sem_wait(&semResfull); // attente d'un slot rempli
    pthread_mutex_lock(&mutexRes);
    // section critique
    for(size_t i=0;i<BUFFER_RES_SIZE;i++)
    {
        if(bufferRes[i])
        {
            strcpy(pswd, bufferRes[i]);
            free(bufferRes[i]);
            bufferRes[i] = NULL;
            r = 1;
            break;
        }
    }
    pthread_mutex_unlock(&mutexRes);
    sem_post(&semResEmpty); // il y a un slot libre en plus
    return r;
}
void freeBufferResSem()
{
    int maxFull = BUFFER_RES_SIZE;
    BUFFER_RES_SIZE = 0;
    for(int i=0;i<maxFull;i++)
        sem_post(&semResfull);
}
