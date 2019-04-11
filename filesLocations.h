#ifndef FILESLOCATIONS_H_INCLUDED
#define FILESLOCATIONS_H_INCLUDED

#include <pthread.h>

typedef struct{
    char **paths;
    int amount;
    int done;
    pthread_t thrd;
}FilesLocation;

int initLocations(const char **, const int);
void freeLocations();
int isAllLocationsDone();

#endif // FILESLOCATIONS_H_INCLUDED
