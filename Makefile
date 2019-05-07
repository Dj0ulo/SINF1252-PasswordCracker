# CC variable for used C compiler
# You can replace gcc by clang if you prefer
CC = gcc
# CFLAGS variable for gcc/clang flags
# See gcc/clang manual to understand all flags
CFLAGS += -std=c99 # Define which version of the C standard to use
CFLAGS += -Wall # Enable the 'all' set of warnings
CFLAGS += -Werror # Treat all warnings as error
CFLAGS += -Wshadow # Warn when shadowing variables
CFLAGS += -Wextra # Enable additional warnings
CFLAGS += -O2 -D_FORTIFY_SOURCE=2 # Add canary code, i.e. detect buffer overflows
CFLAGS += -fstack-protector-all # Add canary code to detect stack smashing


cracker: main.o linkedList.o filesLocations.o files.o buffer.o reverse.o sha256.o
	gcc -o cracker main.o linkedList.o filesLocations.o files.o buffer.o reverse.o sha256.o
main.o: main.c
	gcc -c main.c
linkedList.o: linkedList.c
	gcc -c linkedList.c
filesLocations.o: filesLocations.c
	gcc -c filesLocations.c
files.o: files.c
	gcc -c main.c
buffer.o: buffer.c
	gcc -c buffer.c
reverse.o: hash/reverse.c
	gcc -c reverse.c
sha256.o: hash/sha256.c
	gcc -c sha256.c
