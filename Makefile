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

all : cracker tests	

cracker: main.o linkedList.o filesLocations.o log.o buffer.o bufferRes.o reverse.o sha256.o
	$(CC) $(CFLAGS) -o cracker main.o linkedList.o filesLocations.o log.o buffer.o bufferRes.o reverse.o sha256.o

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c
linkedList.o: src/linkedList.c
	$(CC) $(CFLAGS) -c src/linkedList.c
filesLocations.o: src/filesLocations.c
	$(CC) $(CFLAGS) -c src/filesLocations.c
log.o: src/log.c
	$(CC) $(CFLAGS) -c src/log.c
buffer.o: src/buffer.c
	$(CC) $(CFLAGS) -c src/buffer.c
bufferRes.o: src/bufferRes.c
	$(CC) $(CFLAGS) -c src/bufferRes.c
reverse.o: src/hash/reverse.c
	$(CC) $(CFLAGS) -c src/hash/reverse.c
sha256.o: src/hash/sha256.c
	$(CC) $(CFLAGS) -c src/hash/sha256.c

clean:
	rm -rf *.o cracker
tests: tests/test
	./tests/test
	echo "\n\nTest d'un fichier de 1 hash avec 1 thread\n"
	./cracker tests/test-input/1.bin
	echo "\n\nTest d'un fichier de 1 hash avec 100 threads\n"
	./cracker -t 100 tests/test-input/1.bin
	echo "\n\nTest d'un fichier de 5 hashs avec 5 threads et stockage du résultats dans le fichier out.txt\n"
	./cracker -t 5 -o out.txt tests/test-input/5.bin
	echo "\n\nTest d'un fichier de 5 hashs avec 3 threads et affichage des mots de passe lorsqu'ils sont crackés\n"
	./cracker -t 3 -i tests/test-input/5.bin
	echo "\n\nTest d'un fichier de 1 hash, de 3 hash et de 5 hashs dont certains sont identiques avec 1 thread\n"
	./cracker -t 1 tests/test-input/1.bin tests/test-input/3.bin tests/test-input/5.bin
	echo "\n\nTest d'un fichier de 100 hashs avec 2 threads avec consonne comme critère de sélection\n"
	./cracker -t 2 -c tests/test-input/100.bin
	echo "\n\nTest du fichier de 1000 hashs et de celui de 5 avec 4 threads et avec voyelle comme critère de sélection\n"
	./cracker -t 4 tests/test-input/01_4c_1k.bin tests/test-input/02_6c_5.bin
