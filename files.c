#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

pthread_mutex_t mtxPrt;
void logi(const char* one,const char* two)
{
    pthread_mutex_lock(&mtxPrt);
    printf("%s : %s\n",one,two);
    pthread_mutex_unlock(&mtxPrt);
}
int get32Bytes(uint8_t *buffer, int file, int offset)
{
    if(lseek(file,offset,SEEK_SET)==-1)
        return -2;
    return read(file,buffer,32);
}
