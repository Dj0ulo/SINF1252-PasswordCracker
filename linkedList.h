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

void initList();
void freeList();
void addIfGood(const char *, const int);
void printList();
int writeList(const char *);


#endif // LINKEDLIST_H_INCLUDED
