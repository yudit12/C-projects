#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h> 
#include "Integral.h"
#include <netdb.h> //contain method gethostbyname
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_LENGTH 510

struct integral* getIntegral();

int main (int argc, char* argv[]){
struct sockaddr_in serv_addr;  
struct hostent *server;  

//create new socket
int sockfd= socket(AF_INET, SOCK_STREAM, 0);//(family address,type, protocol)

if (sockfd < 0) { 
	perror("ERROR opening socket");
	exit(1); 
		
	}

//get server Ip
server = gethostbyname(argv[1]); 
if (server == NULL) {         
		fprintf(stderr,"ERROR, no such host\n");         
		exit(1); 
	}
	
//initializa filed on sockaddr_in struct for connection
serv_addr.sin_family = AF_INET; 
serv_addr.sin_port = htons(atoi(argv[2])); // htons()- method that convert short int to network byte order        
bcopy((char *)server->h_addr, (char *)  &serv_addr.sin_addr.s_addr, server->h_length); 



//connect client to  the socket
if (connect(sockfd,(const struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){// if not Succeeded
	perror("ERROR connecting");

	}
// get integral from the user
//struct integral* integ = getIntegral();
char buffer[MAX_LENGTH];
fgets(buffer,MAX_LENGTH,stdin);	

int rc= write(sockfd, buffer, strlen(buffer)+1);
if (rc<0){
perror("ERROR writing");
exit(1);


	}
	
close(sockfd);   	

return 0;
}


