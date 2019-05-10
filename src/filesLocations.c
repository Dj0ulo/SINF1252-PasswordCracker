#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "filesLocations.h"
#include "buffer.h"
#include "log.h"


FilesLocation *locFiles;
int NLOCATIONS = 1;

int parseFiles(FilesLocation *loc)
{
    uint8_t *hash = (uint8_t*)malloc(HASH_SIZE);
    if(!hash){
        logi("Error malloc","parseFiles");
        return -1;
    }
    for(int i=0;i<loc->amount;i++)
    {
        logi("Parsing", loc->paths[i]);

        const char *filename = loc->paths[i];
        int f = open(filename, O_RDONLY);
        if(f>-1)
        {
            struct stat * buf = malloc(sizeof(struct stat));
            if(!buf)
                return -1;
            if(stat(filename, buf)==-1)
                return -1;
            size_t size = buf->st_size;
			free(buf);

            size_t k=0;
            for(;k<size/32;k++)
            {
                int r = read(f,hash,HASH_SIZE);
                if(r==-1)
                    break;
		//printHash("h",hash);
		loc->nbHashs++;
                insertInBuffer(hash);
                lseek(f,0,SEEK_CUR);
            }

            if(close(f)==-1)
                printf("Unable to close a file");
            else
                logi("File closed",loc->paths[i]);
        }
        else
            logi("Unable to open", loc->paths[i]);
    }
    free(hash);
    loc->done = 1;
    return 0;
}

int getDeviceID(const char* filename)
{
    struct stat buf;
    if(stat(filename, &buf)==-1)
        return -1;
    return buf.st_dev;
}

int initLocations(const char **files, const int amount)
{
    NLOCATIONS = 0;
    int devicesID[amount];
    for(int i=0;i<amount;i++)
    {
        devicesID[i] = getDeviceID(files[i]);
        int k=0;
        for(;k<i;k++)
            if(devicesID[k] == devicesID[i])
                break;
        if(k==i)
            NLOCATIONS++;
    }
    locFiles = (FilesLocation*)malloc(NLOCATIONS * sizeof(FilesLocation));
    if(!locFiles){
        logi("Error malloc","initLocations");
        return -1;
    }

    for(int i=0;i<NLOCATIONS;i++){
        locFiles[i].id = -2;
        locFiles[i].done = 0;
		locFiles[i].nbHashs = 0;
    }

    for(int i=0;i<amount;i++)
    {
        for(int k=0;k<NLOCATIONS;k++)
        {
            if(locFiles[k].id == devicesID[i])
                break;
            if(locFiles[k].id==-2)
            {
                locFiles[k].id = devicesID[i];
                break;
            }
        }
    }
    for(int i=0;i<NLOCATIONS;i++)
    {
        locFiles[i].amount=0;
        for(int k=0;k<amount;k++)
            if(locFiles[i].id==devicesID[k])
                locFiles[i].amount++;
        locFiles[i].paths = (const char**)malloc(locFiles[i].amount * sizeof(char*));
        if(locFiles[i].paths)
        {
            for(int k=0,j=0;k<amount;k++)
            {
                if(locFiles[i].id==devicesID[k])
                {
                    locFiles[i].paths[j]=files[k];
                    j++;
                }
            }
        }
        else
        {
            logi("Error malloc","initLocations : paths");
            return -1;
        }
    }
    //printLocations();
    for(int i=0;i<NLOCATIONS;i++)
    {
        int err = pthread_create(&locFiles[i].thrd, NULL, (void*)&parseFiles, (void*)&locFiles[i]);
        if(err)
        {
            logi("Thread creation failed",(const char*)strerror(err));
            return err;
        }
    }
    return 0;
}
void thrdLocationsJoin()
{
	for(int i=0;i<NLOCATIONS;i++)
        pthread_join(locFiles[i].thrd,NULL);
}
void printLocations()
{
    for(int i=0;i<NLOCATIONS;i++)
    {
        printf("-- LOCATION : %d -- %d files -- isDone : %d\n",locFiles[i].id, locFiles[i].amount, locFiles[i].done);
        for(int k=0;k<locFiles[i].amount;k++)
            printf("    %s\n",locFiles[i].paths[k]);
        printf("---------\n");
    }
}
void freeLocations()
{
    if(!locFiles){
        return;
	}
	for(int i=0;i<NLOCATIONS;i++)
	{
		free(locFiles[i].paths);
	}
    free(locFiles);
}
int isAllLocationsDone()
{
    for(int i=0;i<NLOCATIONS;i++)
        if(!locFiles[i].done)
            return 0;
    return 1;
}
int nTotHashs()
{
	int n=0;
	for(int i=0;i<NLOCATIONS;i++)
        n+=locFiles[i].nbHashs;
    return n;
}
