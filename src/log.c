#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>


#include <pthread.h>

pthread_mutex_t mtxPrt;
void logi(const char* one,const char* two)
{
    pthread_mutex_lock(&mtxPrt);
    printf("%s : %s\n",one,two);
    pthread_mutex_unlock(&mtxPrt);
}
void logd(const char* one,const int two)
{
    pthread_mutex_lock(&mtxPrt);
    printf("%s : %d\n",one,two);
    pthread_mutex_unlock(&mtxPrt);
}
