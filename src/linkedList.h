#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    char *value;
    struct Node *next;
};
typedef struct Node node_t;

typedef struct list{
  node_t *first;
  size_t size;
  unsigned int score;
} list_t;

/**
 * Initialise la linked list
 * @param void
 * @return void
 */
void initList();

/**
 * Supprime tous les éléments de la linked list
 * @param void
 * @return void
 */
void eraseList();

/**
 * Libère la linked list de la mémoire
 * @param void
 * @return void
 */
void freeList();

/**
 * Calcule le nombre de voyelles ou de consonnes dans une chaîne de caractères
 * @param const char * : la chaîne de caractères
 * @param const int : critère de sélection VOWEL ou CONSONANT
 * @return unsigned int : nombre de voyelles ou de consonnes dans la chaîne de caractères
 */
unsigned int getScore(const char *, const int);

/**
 * Ajoute un mot de passe à la linked list si il a un score aussi bon ou meilleur que les autres
 * @param const char * : le mot de passe
 * @param const int : critère de sélection VOWEL ou CONSONANT
 * @return int : 0 si c'est le premier de la liste, 1 si il est ajouté, 2 si il est meilleur et donc qu'on a supprimer tous les éléments de la liste au préalable,
 * et  -1 si il est moins bon
 */
int addIfGood(const char *, const int);

/**
 * Recompte le nombre d'éléments dans la linked list
 * @param void
 * @return size_t : le nombre d'éléments dans la linked list
 */
size_t calculateSizeList();

/**
 * Retourne sans recompter le nombre d'éléments dans la linked list
 * @param void
 * @return size_t : le nombre d'éléments dans la linked list
 */
size_t sizeList();

/**
 * Imprime les mots de passe de la linked list
 * @param void
 * @return void
 */
void printList();

/**
 * Ecrit la liste dans un fichier ou dans la sortie standard si aucun nom de fichier n'est envoyé
 * @param const char * : nom du fichier
 * @return int : 0 si tout c'est bien passé, -1 si il y a eu un problème à l'ouverture du fichier, -2 si la linked list est un pointeur null,
 * -3 si il y a eu un problème dans l'écriture, -4 si il y a eu un problème lors de la fermeture
 */
int writeList(const char *);


#endif // LINKEDLIST_H_INCLUDED
