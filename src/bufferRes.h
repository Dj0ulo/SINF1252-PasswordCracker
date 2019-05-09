#ifndef BUFFERRES_H_INCLUDED
#define BUFFERRES_H_INCLUDED

#include <stdint.h>

void initBufferRes();
void freeBufferRes();
int isBufferResEmpty();

void insertInBufferRes(char *);
int removeFromBufferRes(char *);

void freeBufferResSem();

#endif // BUFFERRES_H_INCLUDED
