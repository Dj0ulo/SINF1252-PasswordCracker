#ifndef HASHS_H_INCLUDED
#define HASHS_H_INCLUDED

#define BUFFER_SIZE 10
#define HASH_SIZE 32

void initBuffer();
void freeBuffer();

void insertInBuffer(char *);
char *removeFromBuffer();

#endif // HASHS_H_INCLUDED
