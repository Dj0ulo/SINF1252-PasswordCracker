#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "main.h"
#include "linkedList.h"
#include "constants.h"

pthread_mutex_t mutexList;
list_t *linkedList;

///Node-----------------------------------------------------
node_t *initNode(const char *value)
{
    node_t* n = (node_t*)malloc(sizeof(node_t));
    if(n)
    {
        n->value = (char *)malloc(strlen(value)+1);
        if(!n->value)
        {
            logi("Error malloc","initNode");
            return NULL;
        }
        strcpy(n->value, value);
        n->next = NULL;
        return n;
    }
    return NULL;
}
void freeNode(node_t *node)
{
    if(!node)
        return;
    if(node->value)
        free(node->value);
    free(node);
}
unsigned int getScore(const char *str, const int selection)
{
    int n=0;
    for(int i=0;i<strlen(str);i++)
    {
        if(str[i]=='a' || str[i]=='e' || str[i]=='i' || str[i]=='o' || str[i]=='u' || str[i]=='y'){
            if(selection == VOWEL)
                n++;
        }
        else if(selection == CONSONANT)
            n++;
    }
    return n;
}

///List----------------------------------------------------
void initList()
{
    pthread_mutex_init(&mutexList, NULL);
    linkedList = (list_t*)malloc(sizeof(list_t));
    if(linkedList)
    {
        linkedList->first = NULL;
        linkedList->size = 0;
    }
    else
        logi("Error malloc","initList");
}
void eraseList()
{
    if(!linkedList)
        return;

    node_t *cur = linkedList->first;
    node_t *next = NULL;
    while(cur)
    {
        next = cur->next;
        freeNode(cur);
        cur = next;
    }
    linkedList->size = 0;
    linkedList->first->next = NULL;
    linkedList->first = NULL;
}
void freeList()
{
    if(linkedList)
        return;
    eraseList();
    free(linkedList);
}

int addToList(const char *value, const unsigned int score)
{
    if(linkedList)
    {
        node_t *n = initNode(value);
        if(n==NULL)
            return -1;

        n->next = linkedList->first;
        linkedList->first = n;
        linkedList->score = score;
        linkedList->size++;

        return 1;
    }
    return -1;
}
void resetList(const char *value, const unsigned int score)
{
    if(linkedList)
    {
        eraseList();
        addToList(value, score);
    }
}
void addIfGood(const char *value, const int selection)
{
    pthread_mutex_lock(&mutexList);
    if(linkedList)
    {
        const int n = getScore(value, selection);
        if(linkedList->first==NULL)
            addToList(value, n);
        else
        {
            if(linkedList->score < n)
                resetList(value, n);
            else if(linkedList->score == n)
                addToList(value, n);
        }
    }
    pthread_mutex_unlock(&mutexList);
}
void printList()
{
    pthread_mutex_lock(&mutexList);
    if(linkedList)
    {
        node_t* cur = linkedList->first;
        while(cur)
        {
            printf("%s\n", cur->value);
            cur = cur->next;
        }
    }
    pthread_mutex_unlock(&mutexList);
}

int writeList(const char *filename)
{
    int f = 1;
    if(filename)
        f = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR);
    if(f==-1)
        return -1;
    if(!linkedList)
        return -2;

//    printf("%d\n",linkedList->size);

    node_t* cur = linkedList->first;
    while(cur){
        if(write(f,cur->value,strlen(cur->value))==-1)
            return -3;
        if(write(f,"\n",1)==-1)
            return -3;
        cur = cur->next;
    }
    if(close(f)==-1)
        return -4;
    return 0;
}
