#ifndef FILESLOCATIONS_H_INCLUDED
#define FILESLOCATIONS_H_INCLUDED

#include <pthread.h>

typedef struct{
    const char **paths;
    int amount;
	int nbHashs;
    int done;
    pthread_t thrd;
    int id;
}FilesLocation;

/**
 * Initialise la liste d'emplacements de fichiers
 * @param const char ** : liste totale de fichiers en entrée
 * @param const int : nombre de fichiers en entrée
 * @return int : -1 si un malloc c'est mal passé, 0 sinon
 */
int initLocations(const char **, const int);

/**
 * Attend la fin des threads de lecture dans les fichiers
 * @param void
 * @return void
 */
void thrdLocationsJoin();

/**
 * Imprime la liste d'emplacements de fichiers
 * @param void
 * @return void
 */
void printLocations();

/**
 * Libère la liste d'emplacements de fichiers
 * @param void
 * @return void
 */
void freeLocations();

/**
 * Regarde si tous les fichiers ont été lus en entier
 * @return int : 1 si oui, 0 sinon
 */
int isAllLocationsDone();

/**
 * Renvoie le nombre de hashs lus
 * @return int : nombre de hashs lus
 */
int nTotHashs();

#endif // FILESLOCATIONS_H_INCLUDED
