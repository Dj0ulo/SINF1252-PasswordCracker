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
CFLAGS += -pthread

all : cracker

cracker: main.o linkedList.o filesLocations.o files.o buffer.o bufferRes.o reverse.o sha256.o
	$(CC) $(CFLAGS) -o cracker main.o linkedList.o filesLocations.o files.o buffer.o bufferRes.o reverse.o sha256.o

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c
linkedList.o: src/linkedList.c
	$(CC) $(CFLAGS) -c src/linkedList.c
filesLocations.o: src/filesLocations.c
	$(CC) $(CFLAGS) -c src/filesLocations.c
files.o: src/files.c
	$(CC) $(CFLAGS) -c src/files.c
buffer.o: src/buffer.c
	$(CC) $(CFLAGS) -c src/buffer.c
bufferRes.o: src/bufferRes.c
	$(CC) $(CFLAGS) -c src/bufferRes.c
reverse.o: src/hash/reverse.c
	$(CC) $(CFLAGS) -c src/hash/reverse.c
sha256.o: src/hash/sha256.c
	$(CC) $(CFLAGS) -c src/hash/sha256.c

clean:
	@rm cracke *.o
