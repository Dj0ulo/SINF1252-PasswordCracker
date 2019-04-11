#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>

int get32Bytes(char *buffer, int file, int offset)
{
    if(lseek(file,offset,SEEK_SET)==-1)
        return NULL;
    return read(file,buffer,32);
}
