
The programs realizinga server client interface. 
the goal of the program is calculate the integral of polinom that the user inserts.

In both programs: the server calculate the sum of 32 rectangles between the 'from' and the 'to' 
by 4 suns (each sun calculate 8 rectangles and puts the result in the shared memory).

In questoin 1 the client and the server communicate by shared memory.
the client get input from the user to the following format:   "INTEGRAL, FROM , TO"
as reqested  the input can conain only power of type x^2 and x^1 without coffecent and constant and the limits of the integral 
we will save the if there is power or not in varibale coeff - is array that hold the  0 - there isnt , 1 there is 
and transfer to server by the shared memory.
the server calculate the sum of the 32 rectangles between the 'from' and the 'to' 
from the shared memory.

In questoin 2 the client and the server communicate by socket.
the client write the input in the socket.
the server read the input, dismantle it to the following settings: "from", "to" and "coeff"
and calculate the sum of the 32 rectangles.

The function:
 in integral.h 
 struct integral* constractur():
 creat Integral, allocete memory, set all the thoonot to zero

void printIntegral(struct integral* integ):
print all thoonot of the integral

double calcIntegral(struct integral* integ):
save the sum of the integral at result

struct integral* stringToIntegral(char* arr):
change the input of the user from a string to integral


in ex3q1c.c:

struct integral* getIntegral():
method that get integral from the user (client job)

in ex3q1s.c:
void catch_usrer1(int sig):
calculate the integral by the 4 sons, and share memory 

void catch_c(int sig):
disconect the server


in ex3q2s.c:
void printResult(char* input):
method that calculate the result of the integral by 4 sons and socket
void catch_c(int sig):
disconect from the server 
  

The files of the program:

in files with extension ".h" we :import the needed liberys to use of method
								 saved wanted structures
								 declarations to methods that implementation  in the file with extension ".c" with the same name.
								 
in files with extension ".c" we	: import the file. h with the same name and implementate all  method that relate to the file

in this programe  the files are: Integral.c Integral.h  ex3q1c.c ex3q2c.c ex3q1s.c ex3q2s.c

ex3q1s.c - the server , create shared memory and wait for signal from the client for calcoliting the integral from the shared memory. 
  

ex3qlc.c-
 the client , join to the shared memory dismantle the input to the following settings: "from", "to" and "coeff"
(coeff - is array that hold the coeff of the polinom) transfer all this to server by the shared memory.
and send signal to the server.

ex3q2s.c- the server create socket, read the input, dismantle it to the following settings: "from", "to" and "coeff"
and calculate the sum of the 32 rectangles, by 4 suns.

ex3q2c.c- the client:  join to the socket and write the input in the socket.   

makefile - this file compiling the file type .c and creating "out" files


input:
	the input need to be in the client: 
	polinom, from, to (Separated by commas).

The output will be in the server - the result of the integral
			
how the user  should to compile the program?.
 we create The file that unites all the files together called makefile
this file concerned to compile all the files together.
after saving each file when we write in the terminal "make"  all file pass compiling.


To run the program
command line: "./<the name of the file that the makefile created>"
in this case:
for run the question 1 
	for run the server:  "./ex3q1s char"
	for run the client:  "./ex3q1c char server "
	**the client and in the server must be the same of char.
	
	**one of the ways to know the id of the server is to run the server in this format:
		"./server1 [char]&"	- we get the id of the server and then we must write "fg" in the shell for proper running)

for run the question 2

	for run the server:  "./ex3q2s format (choose num 4 num above 1024) "
	for run the client:  "./ex3q2c [computer name] [format]"
	**in the client and in the server must be the same of format above 1024.
	computer name - is the computer name on which they connect - can be 'localhost'