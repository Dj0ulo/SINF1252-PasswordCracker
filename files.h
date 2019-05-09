#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include <stdint.h>

void logi(const char *,const char *);
void logd(const char *,const int);

//void initLogf();
//void logfi(const char*,const char*);

int get32Bytes(uint8_t *, int, int);

#endif // FILES_H_INCLUDED
