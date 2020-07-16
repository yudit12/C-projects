all: ex3q1c ex3q1s ex3q2c ex3q2s

ex3q1c: ex3q1c.o Integral.o
	gcc -Wall ex3q1c.o Integral.o -o ex3q1c

ex3q1c.o: ex3q1c.c Integral.h
	gcc -Wall -c ex3q1c.c -o ex3q1c.o

ex3q1s: ex3q1s.o Integral.o
	gcc -Wall ex3q1s.o Integral.o -o ex3q1s

ex3q1s.o: ex3q1s.c Integral.h
	gcc -Wall -c ex3q1s.c -o ex3q1s.o

ex3q2c: ex3q2c.o Integral.o
	gcc -Wall ex3q2c.o Integral.o -o ex3q2c

ex3q2c.o: ex3q2c.c Integral.h
	gcc -Wall -c ex3q2c.c -o ex3q2c.o
	
ex3q2s: ex3q2s.o Integral.o
	gcc -Wall ex3q2s.o Integral.o -o ex3q2s

ex3q2s.o: ex3q2s.c Integral.h
	gcc -Wall -c ex3q2s.c -o ex3q2s.o

Integral.o: Integral.c Integral.h
	gcc -Wall -c Integral.c -o Integral.o


clean:
	rm -rf *.o

