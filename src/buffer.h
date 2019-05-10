#ifndef HASHS_H_INCLUDED
#define HASHS_H_INCLUDED

#include <stdint.h>

#define HASH_SIZE 32

/**
 * Initialise le buffer qui contiendra les hashs, ainsi que le mutex et les 2 sémaphores correspondants
 * @param const size_t : sa taille
 * @return void
 */
void initBuffer(const size_t);

/**
 * Libère le buffer de la mémoire
 * @param void
 * @return void
 */
void freeBuffer();

/**
 * Regarde si le buffer est vide
 * @param void
 * @return int : 1 si vide, 0 sinon
 */
int isBufferEmpty();

/**
 * Attend jusqu'à ce qu'il ait une place dans le buffer et y insère un hash
 * @param uint8_t * : le hash à insérer
 * @return void
 */
void insertInBuffer(uint8_t *);

/**
 * Attend jusqu'à ce qu'il ait un hash dans le buffer et l'en retire
 * @param uint8_t * : pointeur vers un emplacement de HASH_SIZE octets dans la mémoire
 * @return void
 */
int removeFromBuffer(uint8_t *);

/**
 * Fait autant de sem_post qu'il y a de place dans le buffer pour libérer d'éventuels threads bloqués attendant de pouvoir retirer un hash
 * @param void
 * @return void
 */
void freeBufferSem();

/**
 * Imprime un hash en hexadécimale
 * @param const char * : texte accompagant l'affichage du hash
 * @param const uint8_t * : le hash
 * @return void
 */
void printHash(const char *,const uint8_t *);


#endif // HASHS_H_INCLUDED
