#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
# include <pthread.h>
#include "Integral.h"


#define MAX_LENGTH 510
#define numOfThread 4 
struct integral* getIntegral();
void* clac_integ_by_thread (void *arg);

int thread_num=0;
int main (int argc, char* argv[]){
 pthread_t thread[numOfThread];
 
integ = getIntegral();
//printIntegral(integ);
//integ->result= calcIntegral(integ);
//printIntegral(integ);

int k= atoi(argv[1]);
int i;
for(i=0; i<numOfThread;i++){
	int status = pthread_create(thread+i,NULL ,clac_integ_by_thread , &k); // &thread[i]= thread+i
	if(status!=0){
		fputs("pthread create failed", stderr);
		exit(EXIT_FAILURE);
		}
	}

// waiting  for all the threads to end 
for(i=0; i<numOfThread;i++){
	double* temp;
	pthread_join(*(thread+i),(void**)&temp);
	integ->result+=*temp;
	free(temp);
}

printf("result is: %lf\n", integ->result);

free(integ);
return 0;	
}



//----------------------------------------------------------------------------------------------------
// method thatdifine each theard is limit of calc in the integral 
void* clac_integ_by_thread (void *arg){
	double dx=(integ->to- integ->from)/numOfThread;// the tvaaxx that each thread need to calc
	int k = *((int*)arg);//  casting to int (int*)arg ant get is value *((int*)arg)
	double* temp =(double*)malloc(sizeof(double));// allocte space in the heap  that all the  thread can get to
	double temp_from, temp_to;//each tread diffrent limit
	temp_from=(thread_num++)*dx+integ->from;// fthread_num++ or advanceing the loop of thread
	temp_to= temp_from+ dx;
	*temp=numericalIntegration(temp_from, temp_to,clac_h, k );
	
	//integ->result+=numericalIntegration(integ->from, integ->to,clac_h, k );
	return temp;
}



//-----------------------------------------------------------------------------------------------------------------
// method that get the input from the user
struct integral* getIntegral(){
	char arr[MAX_LENGTH];
 	fgets(arr,MAX_LENGTH,stdin); 
 	/* arr = "x^2+x^1+c, start, end"
 	   integ->coeff = [1][1][c]
 	   integ->from = start
 	   integ->to = end */
 	return stringToIntegral(arr);
}

//-------------------------------------------------------------------------------------------------------------------------------------------


