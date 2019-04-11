#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "filesLocations.h"
#include "main.h"
#include "buffer.h"
#include "files.h"


FilesLocation *locFiles;
int NLOCATIONS = 1;

int parseFiles(FilesLocation *loc)
{
    uint8_t *buffer = malloc(32);
    if(!buffer){
        printf("Error malloc buffer");
        return -1;
    }
    for(int i=0;i<loc->amount;i++)
    {
        log("Parsing", loc->paths[i]);

        char *filename = loc->paths[i];
        int f = open(filename, O_RDONLY);
        if(f>-1)
        {
            for(int k=0;get32Bytes(buffer, f, k*HASH_SIZE)>0;k++){
                insertInBuffer(buffer);
            }
            if(close(f)==-1)
                printf("Unable to close a file");
        }
        else
            log("Enable to open", loc->paths[i]);
    }
    free(buffer);
    loc->done = 1;
    return 0;
}

int initLocations(const char **files, const int amount)
{
    locFiles = (FilesLocation*)malloc(NLOCATIONS * sizeof(FilesLocation));
    locFiles[0].paths = files;
    locFiles[0].amount = amount;
    locFiles[0].done = 0;

    for(int i=0;i<NLOCATIONS;i++)
    {
        int err = pthread_create(&locFiles[i].thrd, NULL, (void*)&parseFiles, (void*)&locFiles[i]);
        if(err)
        {
            log("Thread creation failed",strerror(err));
            return err;
        }
    }
    return 0;
}
void freeLocations()
{
    if(!locFiles)
        return;
    free(locFiles);
}
int isAllLocationsDone()
{
    for(int i=0;i<NLOCATIONS;i++)
        if(!locFiles[i].done)
            return 0;
    return 1;
}
