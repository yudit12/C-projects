
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct integral {
	double coeff[3];
	double from;
	double to;
	double result;
};

struct integral* constractur(); // creat Integral, allocete memory, set all the thoonot to zero

void printIntegral(struct integral* integ); // print all thoonot of the integral

double calcIntegral(struct integral* integ); // save the sum of the integral at result

struct integral* stringToIntegral(char* arr);

 
