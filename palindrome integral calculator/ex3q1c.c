#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "Integral.h"
#include <signal.h>

#define MAX_LENGTH 510

struct integral* getIntegral();


int main (int argc, char* argv[]){
 // key for part of  creating the share memory	
key_t key;
if((key=ftok("/tmp",argv[1][0]))==-1){
	perror( "ftok faild\n");
	exit(EXIT_FAILURE);
}
 //printf("%d\n", key);
// allocat to the memory how many memmory we needed and  arshaot
int shm_id;
shm_id=shmget(key,sizeof(struct integral), 0600);
if(shm_id==-1){
 	perror( "shmget faild\n");
	exit(EXIT_FAILURE);
 
 }

 // conect between my program to the memmory- create pointer to the shre memory that knows to write and read integral
struct integral* shm_ptr;// pointer type integral 
 shm_ptr=(struct integral*) shmat(shm_id,NULL,0);// smat return pointer type void threfre we are doing casting to  pointer to integral
 if(shm_ptr== (struct integral*)-1){
 	perror("shmat faild");
 	exit(EXIT_FAILURE);
 }
 
struct integral* integ = getIntegral();

// write our integral to share memory 
*shm_ptr= *integ;// compere data
printIntegral(shm_ptr);
//send signal to the server that we write the integral
int pid_p1= atoi(argv[2]);
kill(pid_p1 ,SIGUSR1); // send signal (id prosess 1, signal type)

//after writing to the share memorey the client need to disconnect from the share memmory
shmdt(shm_ptr);// delelite the pointer that conect the program to the share memmory

free(integ);
return 0;
}


//------------------------------------------------------------------------------------------------------------------------------------------
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


