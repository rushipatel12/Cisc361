mp3: mp3.o
	gcc -g mp3.o -o mp3
mp3.o: mp3.c mp3.h
	gcc -g -c mp3.c -o mp3.o
clean: 
	rm *.o mp3
run:
	./mp3
