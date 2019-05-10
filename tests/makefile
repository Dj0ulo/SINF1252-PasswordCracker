test: main.o linkedList.o log.o bufferRes.o
	gcc -pthread -o test main.o linkedList.o log.o bufferRes.o -lcunit
main.o: main.c
	gcc -c main.c
linkedList.o: ../src/linkedList.c
	gcc -c ../src/linkedList.c
log.o: ../src/log.c
	gcc -c ../src/log.c
bufferRes.o: ../src/bufferRes.c
	gcc -pthread -c ../src/bufferRes.c
	
clean:
	rm -rf *.o test
