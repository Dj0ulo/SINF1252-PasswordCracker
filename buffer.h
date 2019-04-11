#ifndef HASHS_H_INCLUDED
#define HASHS_H_INCLUDED

#define HASH_SIZE 32

void initBuffer();
void freeBuffer();
int isBufferEmpty();

void insertInBuffer(uint8_t *);
uint8_t *removeFromBuffer();

void printHash(const char *,const uint8_t *);


#endif // HASHS_H_INCLUDED
