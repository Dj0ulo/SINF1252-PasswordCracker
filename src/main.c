#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <getopt.h>
#include <time.h>

#include "hash/reverse.h"

#include "linkedList.h"
#include "buffer.h"
#include "bufferRes.h"
#include "log.h"
#include "filesLocations.h"
#include "constants.h"


extern pthread_mutex_t mtxPrt;
int SELECTION = VOWEL;
int PRINT_HASH = 0;

bool thrdsHashDone = false;
void crackHashes()
{
    uint8_t *hash = (uint8_t*)malloc(HASH_SIZE);
    if(!hash)
    {
        logi("Error malloc","hash");
        return;
    }
    char res[MAX_SIZE_PSWD + 1];
    while(1)
    {
        if(isAllLocationsDone() && isBufferEmpty())//si plus de hash a cracker
        {
            freeBufferSem();
            break;
        }
        //logi("Wait","HASH");
        if(removeFromBuffer(hash))
        {
            //printHash("Get",hash);
            res[0]=0;
            int succes = reversehash(hash, res, MAX_SIZE_PSWD);

            if(succes){
                insertInBufferRes(res);
				if(PRINT_HASH)
                	logi("ReverseHash has found",res);
            }
            else
                logi("ReverseHash didn't find a password that matches the hash...",res);
        }
    }
    //logi("CRACK HASH","FINISH");
    free(hash);
}
void sortPswd()
{
    char *pswd  = (char*)malloc(MAX_SIZE_PSWD + 1);
    if(!pswd)
    {
        logi("Error malloc","pswd");
        return;
    }
    while(1){
        if(thrdsHashDone && isBufferResEmpty())//si plus de pswd a trier
            break;
        //logi("SORT PASSWORD","Wait");
        if(removeFromBufferRes(pswd))
        {
            //logi("Sorting",pswd);
            addIfGood(pswd, SELECTION);
        }
    }
    //logi("SORT PASSWORD","FINISH");
    free(pswd);
}

int main(int argc, char *argv[])
{
	clock_t  start = clock();
    pthread_mutex_init(&mtxPrt, NULL);

    printf("--- Cracker ---\n");

    int NTHREADS = 1;
    const char *OUT_FILE = NULL;
    const char **IN_FILES = NULL;
    int N_IN_FILES = 1;

    //Source : https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt
    char c;
    while ((c = getopt (argc, argv, "ict:o:")) != -1){
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
		case 'i':
            PRINT_HASH = 1;
            break;
        case '?':
            return 1;
        }
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
    char *in = "02_6c_5.bin";
    IN_FILES[0] = in;
    char *out = "out.txt";
    OUT_FILE = out;
    NTHREADS = 10;*/

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
    initBufferRes(NTHREADS*2);
    int err = initLocations(IN_FILES, N_IN_FILES);
    if(err)
        return err;
    initList();

    pthread_t thrdCalcul[NTHREADS];
    for(int i=0;i<NTHREADS;i++)
    {
        int errt = pthread_create(&thrdCalcul[i], NULL, (void*)&crackHashes, NULL);
        if(errt)
        {
            logi("Thread creation failed",strerror(err));
            return err;
        }
    }
    pthread_t thrdSort;
    int errt = pthread_create(&thrdSort, NULL, (void*)&sortPswd, NULL);
    if(errt)
    {
        logi("Thread creation failed",strerror(err));
        return err;
    }

	//WAIT threads
	thrdLocationsJoin();
	if(isBufferEmpty())
		freeBufferSem();

    for(int i=0;i<NTHREADS;i++)
        pthread_join(thrdCalcul[i],NULL);
    thrdsHashDone = true;

	if(isBufferResEmpty())
    	freeBufferResSem();

    pthread_join(thrdSort,NULL);

    writeList(OUT_FILE);

	logd("Number of password",sizeList());

    freeList();
    freeLocations();
    freeBufferRes();
    freeBuffer();

    free(IN_FILES);

    logd("Finish in (ms)",1000*(clock()-start)/CLOCKS_PER_SEC);

    return 0;
}


