
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <signal.h>
#include "Integral.h"

#define  MAX_LENGTH 510
#define numOfSons 4

void printResult(char* input);
void catch_c(int sig);
int sockfd, newsockfd;    

int main(int argc, char *argv[]) {
signal(SIGINT,catch_c);// signal type, which method hndler it
int sockfd, newsockfd;    
char buffer[MAX_LENGTH]; //  save the string that client send   
struct sockaddr_in serv_addr; // alll server database
struct sockaddr cli_addr; // all client database
socklen_t clilen;//client sockt length   

//create socket
sockfd = socket(AF_INET,SOCK_STREAM,0); 
	if (sockfd < 0){  
		perror("ERROR opening socket");
		}

//initializa filed on sockaddr_in struct for connection
serv_addr.sin_family = AF_INET;  
serv_addr.sin_addr.s_addr = INADDR_ANY; 
serv_addr.sin_port = htons(atoi(argv[1])); // it what conect between the client and the server  
	
// give socket a name
if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)                 
		perror("ERROR on binding");  

// waiting for connection
listen(sockfd,5); //5-  maxsimon of 5 client can connect 





//-------> from this point on the server start be active


while(1){// loop that waiting to conection of new client
// concet to a new client
	newsockfd = accept(sockfd, &cli_addr, &clilen); 
	if (newsockfd < 0) {                
		perror("ERROR on accept"); 
		
		} 
	int howManyByteRead=read(newsockfd, buffer,MAX_LENGTH);// the method read knows that it need to wait for ending writing 
	if(howManyByteRead<0){
		  perror("read from sockt failed") ;
                  exit(EXIT_FAILURE) ;
	
	}
// if  we sessucfuly read from the sockt
	printResult(buffer);
	close(newsockfd); // disconect the client
}



return 0;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------
void printResult(char* input){
struct integral* integ = stringToIntegral(input);
//calc by 4 sons

int i;
pid_t id;
double dx=(integ->to- integ->from)/4;
int pipes[2];
if(pipe(pipes) < 0) {
	perror("pipe failed") ;
        exit(EXIT_FAILURE) ;
}

for (i=0; i<numOfSons;i++){
	id=fork();
	if(id == 0) {
		close(pipes[0]); // close reading
		// create diffrent limit of the integral for each son
		integ->from=i*dx+integ->from;
		integ->to= integ->from+ dx;
		
		double calc = calcIntegral(integ);
		// write the result to the pipe
		write(pipes[1], &calc, sizeof(double));
		close(pipes[1]); // close write
		// son finsh is job  so we need to finsh all and kill the son 
		close(newsockfd); // disconect the client
		free(integ);
		exit(0);
	}
	
}
close(pipes[1]); // close writing
 for (i=0; i<numOfSons;i++){
 	double result=0;
	read(pipes[0], &result, sizeof(double));
	integ->result+=result;
}
close(pipes[0]); // close reading
printf(" integral result : %lf\n", integ->result);
free(integ);

}
//--------------------------------------------------------------------------------------------------------------------------------------------
void catch_c(int sig){
	close(sockfd);
	exit(0);//end program
}

