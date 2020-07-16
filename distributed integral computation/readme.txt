


the goal of the program is calculate the integral of polinom that the user insert by four threads .
the program will get value (k) that each thread will calc


the porogram get input from the user to the following format:   "INTEGRAL, FROM , TO"
as reqested  the input can conain only power of type x^2 and x^1 without coffecent and one  constant and the limits of the integral 
we will save the if there is power or not in varibale coeff - is array that hold the  0 - there isnt , 1 there is 
each power can appear no more the one time

afther all the thread will finsh ther calc  we will sum al the clac  thogter and print it



The function:
 in integral.h 
 struct integral* constractur():
 create Integral, allocete memory, set all the thoonot to zero

void printIntegral(struct integral* integ):
print all thoonot of the integral

double numericalIntegration(double a, double b,double (*calc_h)(double), int k ):
method that calc the the integral

double clac_h(double x): method that the func above get to calc the integral  this method calc the hight of the integral

struct integral* stringToIntegral(char* arr):
change the input of the user from a string to integral


in ex4q1:
void* clac_integ_by_thread (void *arg):
method that diffine each theard is limit of calc in the integral 

struct integral* getIntegral():
// method that get the input from the user

The files of the program:

in files with extension ".h" we :import the needed liberys to use of method
								 saved wanted structures
								 declarations to methods that implementation  in the file with extension ".c" with the same name.
								 
in files with extension ".c" we	: import the file. h with the same name and implementate all  method that relate to the file

in this programe  the files are: Integral.c Integral.h  ex4.c


  

makefile - this file compiling the file type .c and creating "out" files


input:
	the input need to be in the 
	polinom, from, to (Separated by commas).

The output will be in the server - the result of the integral
			
how the user  should to compile the program?.
 we create The file that unites all the files together called makefile
this file concerned to compile all the files together.
after saving each file when we write in the terminal "make"  all file pass compiling.


To run the program
command line: "./ex4 [k]  k= the size you want to divid the integral calc for each thread
 after you can enter the wanted (and havble)integral for calclition
