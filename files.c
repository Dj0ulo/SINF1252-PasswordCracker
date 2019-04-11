#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>

int get32Bytes(uint8_t *buffer, int file, int offset)
{
    if(lseek(file,offset,SEEK_SET)==-1)
        return -2;
    return read(file,buffer,32);
}
