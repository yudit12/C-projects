
ex4: chatserver.o list.o
	gcc -g chatserver.o list.o -o server

chatserver.o: chatserver.c slist.h
	gcc -Wall -g -c chatserver.c -o chatserver.o

list.o: slist.c slist.h
	gcc -Wall -g -c slist.c -o list.o

clean:
	rm -rf *.o



