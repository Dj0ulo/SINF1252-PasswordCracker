#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#include "hash/reverse.h"

#include "linkedList.h"
#include "buffer.h"
#include "files.h"
#include "filesLocations.h"
#include "constants.h"

//https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt


pthread_mutex_t mtxPrt;
int SELECTION = VOWEL;

void errMalloc(const char* error)
{

}
void logi(const char* one,const char* two)
{
    pthread_mutex_lock(&mtxPrt);
    printf("%s : %s\n",one,two);
    pthread_mutex_unlock(&mtxPrt);
}

void crackHashes()
{
    uint8_t *hash;
    char res[MAX_SIZE_PSWD + 1];
    while(1){
        if(isAllLocationsDone() && isBufferEmpty())//si plus de hash a cracker
            break;
        hash = removeFromBuffer();
        if(hash){
            sprintf(res,"");
            int succes = reversehash(hash, res, MAX_SIZE_PSWD);

            if(succes){
                addIfGood(res, SELECTION);
                logi("ReverseHash has found",res);
            }
            else
                logi("ReverseHash didn't find a password that matches the hash...",res);
        }
    }
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mtxPrt, NULL);
    printf("Hello world!\n");

    int NTHREADS = 1;
    const char *OUT_FILE = NULL;
    const char **IN_FILES = NULL;
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
    IN_FILES = (const char **)malloc(N_IN_FILES * sizeof(char*));
    if(!IN_FILES)
    {
        logi("Error malloc","IN_FILES");
        return -1;
    }
    for(int i=optind;i<argc;i++)
        IN_FILES[i-optind] = argv[i];

    /*N_IN_FILES = 1;
    IN_FILES = malloc(N_IN_FILES * sizeof(char*));
    char *in = "test.bin";
    IN_FILES[0] = in;
    char *out = NULL;//"out.txt";
    OUT_FILE = out;
    NTHREADS = 6;*/

    ///PRINT-------------------------------------
    printf("NTHREADS : %d\n",NTHREADS);
    for(int i=0;i<N_IN_FILES;i++)
        printf("IN FILE n_%d : %s\n",i+1,IN_FILES[i]);
    printf("OUT FILE : %s\n",OUT_FILE);
    if(SELECTION == VOWEL)
        printf("SELECTION : VOWEL\n");
    else
        printf("SELECTION : CONSONANT\n");

    ///------------------------------------------

    initBuffer(NTHREADS*2);
    int err = initLocations(IN_FILES, N_IN_FILES);
    if(err)
        return err;
    initList();

    pthread_t thrdCalcul[NTHREADS];
    for(int i=0;i<NTHREADS;i++)
    {
        int err = pthread_create(&thrdCalcul[i], NULL, (void*)&crackHashes, NULL);
        if(err)
        {
            logi("Thread creation failed",strerror(err));
            return err;
        }
    }
    for(int i=0;i<NTHREADS;i++)
        pthread_join(thrdCalcul[i],NULL);

    err = writeList(OUT_FILE);
    if(err)
        return err;

    freeList();
    freeLocations();
    freeBuffer();

    free(IN_FILES);

    return 0;
}


