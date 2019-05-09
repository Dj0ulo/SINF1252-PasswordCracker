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

int f;
pthread_mutex_t mtxLogF;
void initLogf(){
    f = open("log.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IWRITE);
}
void logfi(const char* one,const char* two)
{
    pthread_mutex_lock(&mtxLogF);
    char r[100];
    sprintf(r,"%s : %s\n",one,two);
    write(f,r,strlen(r));
    pthread_mutex_unlock(&mtxLogF);
}
int get32Bytes(uint8_t *buffer, int file, int offset)
{
    if(lseek(file,offset,SEEK_SET)==-1)
        return -2;
    return read(file,buffer,32);
}
