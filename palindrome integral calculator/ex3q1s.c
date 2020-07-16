#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include "Integral.h"
#include <signal.h>
#include <unistd.h>

#define numOfSons 4

void catch_c(int sig);
void catch_usrer1(int sig);
int shm_id;
struct integral* shm_ptr;// pointer type integral 




int main (int argc, char* argv[]){

signal(SIGINT,catch_c);// signal type, which method hndler it
signal(SIGUSR1,catch_usrer1);
 // key for part of  creating the share memory	
key_t key;
if((key=ftok("/tmp",argv[1][0]))==-1){
	perror( "ftok faild\n");
	exit(EXIT_FAILURE);
}
 printf("%d\n", key);
// allocate to the memory how many memmory we needed and  arshaot

shm_id=shmget(key,sizeof(struct integral), IPC_CREAT|IPC_EXCL|0600);
if(shm_id==-1){
 	perror( "shmget faild\n");
	exit(EXIT_FAILURE);
 
 }
 
 // conect between my program to the memmory- create pointer to the shre memory that knows to write and read integral

 shm_ptr=(struct integral*) shmat(shm_id,NULL,0);// smat return pointer type void threfre dwe are doing casting to  pointer to integral
 if(shm_ptr== (struct integral*)-1){
 	perror("shmat faild");
 	exit(EXIT_FAILURE);
 }
 
 // inter to endless loop waiting for signal that was wirten intgral to the share memmory
 while(1);

return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// if we in this method it means that the cliend end to write to the share memmory

void catch_usrer1(int sig){
printIntegral(shm_ptr);
// read the integral from the share memmory
struct integral* integ_temp = constractur();// create new integral
*integ_temp = *shm_ptr;// copy the data from the share memmory
//*
//calc it by 4 sons 
int i;
pid_t id;
double dx=(integ_temp->to- integ_temp->from)/4;
for (i=0; i<numOfSons;i++){
	id=fork();
	if(id == 0) {
		// create diffrent limit of the integral for each son
		integ_temp->from=i*dx+integ_temp->from;
		integ_temp->to= integ_temp->from+ dx;
		// write the result to the share memmory
		shm_ptr->result+=calcIntegral(integ_temp);
		// son finsh is job  so we need to finsh all and kill the son 
		free(integ_temp);
		shmdt(shm_ptr);
		exit(0);
	}
	
}
 for (i=0; i<numOfSons;i++){
	waitpid(-1, NULL, 0);//  papa weakup only if one of is son die

}
printf(" integral result : %lf\n", shm_ptr->result);
free(integ_temp);


}

//----------------------------------------------------------------------------------------------------------------------------
//for disconect the server
void catch_c(int sig){
    shmdt(shm_ptr);// disconect from the share memmory
    struct shmid_ds shm_desc;//  struct shmid_ds  of database on the share  memmory
    if(shmctl(shm_id, IPC_RMID, &shm_desc) == -1) {// delete share memmory
        perror("shmctl failed");
    }
	exit(0);//end program
}
