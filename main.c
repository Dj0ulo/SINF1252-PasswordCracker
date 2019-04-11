#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt

#define VOWEL 0
#define CONSONANT 1


int main(int argc, char *argv[])
{
    printf("Hello world!\n");

    int NTHREADS = 1, SELECTION = VOWEL;
    char *OUT_FILE = NULL;
    char **IN_FILES = NULL;
    int N_IN_FILES = 1;

    char c;
    while ((c = getopt (argc, argv, "ct:o:")) != -1)
        switch (c)
        {
        case 't':
            NTHREADS = atoi(optarg);
            if(NTHREADS<=0)
                NTHREADS = 1;
            break;
        case 'c':
            SELECTION = CONSONANT;
            break;
        case 'o':
            OUT_FILE = optarg;
            break;
        case '?':
            return 1;
        }
    N_IN_FILES = argc-optind;
    IN_FILES = malloc(sizeof(char*) * N_IN_FILES);
    for(int i=optind;i<argc;i++)
        IN_FILES[i-optind] = argv[i];

    ///PRINT-------------------------------------
    printf("NTHREADS : %d\n",NTHREADS);
    for(int i=0;i<N_IN_FILES;i++)
        printf("IN FILE n_%d : %s\n",i+1,IN_FILES[i]);
    printf("OUT FILE : %s\n",OUT_FILE);
    ///------------------------------------------



    return 0;
}
