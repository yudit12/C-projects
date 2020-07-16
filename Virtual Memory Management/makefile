	
ex5q1: main.o mem_sim.o
	gcc -Wall main.o mem_sim.o -o ex5q1

main.o: main.c mem_sim.h
	gcc -Wall -c main.c -o main.o


mem_sim.o: mem_sim.c mem_sim.h
	gcc -Wall -c mem_sim.c -o mem_sim.o

		
clean:
	rm -rf *.o

