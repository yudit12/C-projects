
#include "Integral.h"
//------------------------------------------------------------------------------------------------------------------------------------------
struct integral* constractur(){ // creat Integral, allocete memory, set all the thoonot to zero
	struct integral* integ = (struct integral*) malloc (sizeof(struct integral));
	// se-t to zero
	integ->from=0;
	integ->to=0;
	integ->result=0;
	int i=0;
	for(;i<3;i++){
		integ->coeff[i]=0;
	}
	
	return integ;
}

//-----------------------------------------------------------------------------------------------------------------------------------------

void printIntegral(struct integral* integ){ // print all thoonot of the integral
	if (integ->coeff[0]!=0){
	
		printf( "x^2+");
		printf( "x^1+");
	
	}
	if(integ->coeff[2]!=0){
		printf("%lf\n",integ->coeff[2]);
	
	}
	printf("from: %lf to: %lf\n", integ->from, integ->to);
	printf("result: %lf\n" ,integ->result);
	
}



//----------------------------------------------------------------------------------------------------------------------------------------------

double calcIntegral(struct integral* integ){ // calc by riman sum save the sum of the integral at result
	double x;
	double dx =(integ->to-integ->from)/8;
	double temp=0;	
	for(x= integ->from;x<integ->to;x+=dx){
		temp+=(integ->coeff[0]*x*x+integ->coeff[1]*x+integ->coeff[2])*dx;//calc te polynom
	
	}
	return temp;
	
}


//--------------------------------------------------------------------------------------------------------------------------------------------

struct integral* stringToIntegral(char* arr){
	struct integral* integ = constractur();
	char* temp = arr;
 	if(strstr(arr,"x^2") != NULL) {
 		integ->coeff[0] = 1;
 		temp += 4;
 	}  
 	
 	else {
 		integ->coeff[0] = 0;
 	}
 	
 	if(strstr(arr,"x^1") != NULL) {
 		integ->coeff[1] = 1;
 		temp += 4;
 	}  
 	
 	else {
 		integ->coeff[1] = 0;
 	}
 	
 	if(*temp != ' ') { // if defrent so we have coeff without x
 		integ->coeff[2] = strtod(temp, &temp);// func of string to double
 		/* temp = "18.5, 3, 5"
 		   integ->coeff[2] = strtod(temp, &temp);
 		   integ->coeff[2] = 18.5
 		   temp = ", 3, 5" */
 		temp += 2;
 	}
 	
 	else { // if equls so we haven't coeff without x
 		integ->coeff[2] = 0;
 		temp++;
 	}
 	
 	integ->from = strtod(temp, &temp);// func of string to double
 	temp += 2;
 	integ->to = strtod(temp, NULL);// func of string to double
 	
 	return integ;
}

//-------------------------------------------------------------------------------------------------------------------------------------

