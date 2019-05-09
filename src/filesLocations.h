#ifndef FILESLOCATIONS_H_INCLUDED
#define FILESLOCATIONS_H_INCLUDED

#include <pthread.h>

typedef struct{
    const char **paths;
    int amount;
    int done;
    pthread_t thrd;
    int id;
}FilesLocation;

int initLocations(const char **, const int);
void thrdLocationsJoin();
void printLocations();
void freeLocations();
int isAllLocationsDone();

#endif // FILESLOCATIONS_H_INCLUDED
