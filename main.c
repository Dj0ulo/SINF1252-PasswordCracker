#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#include "buffer.h"
#include "files.h"

//https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt

#define VOWEL 0
#define CONSONANT 1

pthread_mutex_t mtxPrt;

typedef struct{
    char **paths;
    int amount;
}FilesList;

int parseFiles(FilesList *files)
{
    char *buffer = malloc(32);
    if(!buffer){
        printf("Error malloc");
        return -1;
    }
    for(int i=0;i<files->amount;i++)
    {
        pthread_mutex_lock(&mtxPrt);
        printf("Parsing : %s\n",files->paths[i]);
        pthread_mutex_unlock(&mtxPrt);

        char *filename = files->paths[i];
        int f = open(filename, O_RDONLY);
        if(f>-1)
        {
            for(int k=0;get32Bytes(buffer, f, k*HASH_SIZE)>0;k++)
                insertInBuffer(buffer);
            if(close(f)==-1)
                printf("Unable to close a file");
        }
    }
    free(buffer);
    return 0;
}
int crackHashes()
{

}

int main(int argc, char *argv[])
{
    printf("Hello world!\n");

    int NTHREADS = 1, SELECTION = VOWEL;
    char *OUT_FILE = NULL;
    char **IN_FILES = NULL;
    int N_IN_FILES = 1;

    char c;
    while ((c = getopt (argc, argv, "ct:o:")) != -1)
        switch (c)
        {
        case 't':
            NTHREADS = atoi(optarg);
            if(NTHREADS<=0)
                NTHREADS = 1;
            break;
        case 'c':
            SELECTION = CONSONANT;
            break;
        case 'o':
            OUT_FILE = optarg;
            break;
        case '?':
            return 1;
        }
    N_IN_FILES = argc-optind;
    IN_FILES = malloc(N_IN_FILES * sizeof(char*));
    for(int i=optind;i<argc;i++)
        IN_FILES[i-optind] = argv[i];

    ///PRINT-------------------------------------
    printf("NTHREADS : %d\n",NTHREADS);
    for(int i=0;i<N_IN_FILES;i++)
        printf("IN FILE n_%d : %s\n",i+1,IN_FILES[i]);
    printf("OUT FILE : %s\n",OUT_FILE);
    ///------------------------------------------

    /*int f = open(filename, O_RDONLY);
    if(f>-1)
    {
        struct stat * buf = malloc(sizeof(struct stat));
        if(fstat(f,buf)==-1)
            return -1;
        if(buf->st_size<index)
            return -2;
        if(lseek(f,index*sizeof(int),SEEK_SET)==-1)
            return -1;
        int b;
        if(read(f,&b,sizeof(int))==-1)
            return -1;
        if(close(f)==-1)
            return -1;
        return b;
    }
    return -1;*/

    pthread_mutex_init(&mtxPrt, NULL);


    const int N_LOCATIONS = 1;
    pthread_t thrdFiles[N_LOCATIONS];

    FilesList filesList;
    filesList.paths = IN_FILES;
    filesList.amount = N_IN_FILES;

    for(int i=0;i<N_LOCATIONS;i++)
    {
        int err = pthread_create(&thrdFiles[i], NULL, (void*)&parseFiles, (void*)&filesList);
        // Check if thread is created sucessfuly
        if (err)
        {
            printf("Thread creation failed : %s",strerror(err));
            return err;
        }
        else{
            pthread_mutex_lock(&mtxPrt);
            printf("Thread Created with ID : %d\n", thrdFiles[i]);
            pthread_mutex_unlock(&mtxPrt);
        }
    }

    pthread_join(thrdFiles[0],NULL);

    free(IN_FILES);
    return 0;
}


