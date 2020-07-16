	
ex4q1: ex4q1.o Integral.o
	gcc -Wall ex4q1.o Integral.o -o ex4 -pthread

ex4q1.o: ex4q1.c Integral.h
	gcc -Wall -c ex4q1.c -o ex4q1.o


Integral.o: Integral.c Integral.h
	gcc -Wall -c Integral.c -o Integral.o

		
clean:
	rm -rf *.o

