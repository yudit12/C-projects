#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <time.h>
#include <ctype.h>
#include <netdb.h>//contain method gethostbyname
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"
#define MIN_ARG 2
#define MAX_ARG 5
#define MAX_COLOM 3
#define MIN_COLOM 2
#define BUF_SIZE 128

time_t now;
//-------------------------------------------------------------------------------------------
char*  send_request( char* h_flag,char*d_flag,char* path,char* host, char *timebuf);
void bad_usage(char* msg);
char * calc_time_interval (int *date);
//-------------------------------------------------------------------------------

int main(int argc, char **argv) {

	if (argc< MIN_ARG || argc> MAX_ARG){
		fprintf(stderr, "Usage: client [-h] [-d <time-interval>] <URL>\n");
		exit(EXIT_FAILURE);

	}
	char* timebuf=NULL;
	char * d_flag ="";
	char* h_flag="";
	int find_url=0;
	char * path="";
	int port;
	int i=1;// i=0 ->./client
	char* tempStr;
	char * host="";// host that include the port if need to (if port!80)
	char *send_host="";// host that we send to the socket

	for(;i<argc;i++){// run on all the arguments
		tempStr=argv[i];
		if(strcmp("-h",tempStr)==0){
			if(strcmp("-h",h_flag)==0){// allready have -h
				bad_usage("Usage: client [-h] [-d <time-interval>] <URL>\n");
			}
			h_flag=tempStr;

		}//---------------------------------------------------------------
		else if(strcmp("-d",tempStr)==0){
			if(strcmp("-d",d_flag)==0){// allready have -h
				bad_usage("Usage: client [-h] [-d <time-interval>] <URL>\n");
			}
			d_flag=tempStr;
			// check if after that we get the <time-interval>
			if(i+1>=argc){
				bad_usage("Usage: client [-h] [-d <time-interval>] <URL>\n");
			}
			char* nextStr=argv[i+1];
			int j=0;
			int k=0;
			int date[3];
			char *token = strtok(nextStr, ":");
			while( token != NULL ) {
				for(j=0;j<strlen(token);j++){
					if(isdigit(token[j])==0 && token[0]!='-'){
						bad_usage("Usage: client [-h] [-d <time-interval>] <URL>\n");
					}
				}
				if(k>MAX_COLOM){// three ":" or more
					bad_usage("wrong input\n");
				}
				date[k]=atoi(token);
				k+=1;
				token = strtok(NULL,":");
			}
			if(k<=MIN_COLOM){
				bad_usage("wrong input\n");
			}
			// successfully saperid the time interval
			timebuf=calc_time_interval(date);
			i++;

		}//--------------------------------------------------------------------------------
		else {// in the URL part
			find_url=+1;
			int j=0;
			if(strncmp("http://",argv[i],7)!=0){
				bad_usage("Usage: client [-h] [-d <time-interval>] <URL>\n");
			}
			char* tempStr=argv[i];
			char* check_path= strchr(tempStr+7,'/');
			if(check_path==NULL){
				bad_usage("wrong input\n");
			}
			path=(char*)calloc(strlen(check_path)+1,sizeof(char));
			if(path==NULL){
				perror("calloc");
				exit(EXIT_FAILURE);
			}
			strcpy(path,check_path);// save the path
			char *token = strtok(tempStr+7, "/");// change to '/0'
			if(token==NULL){
				bad_usage("wrong input\n");
			}
			//find if there is given port in the url
			char *dots_place= strchr(token,':');// return string that start from the first ':'
			send_host=(char*)calloc(strlen(token)+1,sizeof(char));
			if(send_host==NULL){
				perror("calloc");
				exit(EXIT_FAILURE);
			}
			//if no ":"- no port
			if(dots_place==NULL){
				port=80;
				host= token;
				strncpy(send_host,token,strlen(token));// shave the host

			}
			//if there is port
			else{
				if(strcmp(dots_place+1,"80")==0){
					host=strtok(token,":");
					strncpy(send_host,token,strlen(token));
				}
				else{
					host=strtok(token,"/");
					// the send_host will set after

				}
				char*port_str=strtok(dots_place+1, "\0");//return string after char ":"
				if(port_str==NULL){
					bad_usage("wrong input\n");
				}
				for(j=0;j<strlen(port_str);j++){
					if(isdigit(port_str[j])==0){
						bad_usage("wrong input\n");
					}
				}
				port=atoi(port_str);
				if(path==NULL){// we have / after the host
					path="/";

				}
				if(strcmp(send_host,"")==0){// if the host didnt set yet
					send_host=(char*)calloc((strlen(token)-strlen(dots_place))+1,sizeof(char));
					if(send_host==NULL){
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					//save the host to send
					strncpy(send_host,host,strlen(token)-strlen(dots_place));
				}

			}// end of care of port in the url
			//---------------------------------------------------------------------------------------
		}//end of else- care of URL
	}

	if(find_url!=1){
		bad_usage("Usage: client [-h] [-d <time-interval>] <URL>\n");
	}
	/// get the request
	char* request=send_request(  h_flag,d_flag,path, host,timebuf);
	free(timebuf);
	free(path);

	//printf("HTTP request =\n%s\nLEN = %d\n", request, (int)strlen(request));



	//--------------------------------------------------------------------------------------
	//creates socket and connects
	//create new socket

	int sockfd= socket(PF_INET, SOCK_STREAM, 0);//(family address,type, protocol)
	//PF_INET: associates a socket with the Internet protocol family
	//SOCK_STREAM: selects the TCP protocol

	if (sockfd < 0) {
		perror("socket");
		free(request);
		exit(EXIT_FAILURE);

	}
	struct hostent *server;

	server = gethostbyname(send_host);////get server Ip

	if (server == NULL) {
		herror("gethostbyname");
		free(request);
		exit(EXIT_FAILURE);
	}
	//printf("%s",host);
	struct sockaddr_in serv_addr;


	//initializa filed on sockaddr_in struct for connection
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port); // htons()- method that convert short int to network byte order
	//bcopy((char *)server->h_addr, (char *)  &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_addr.s_addr = ((struct in_addr*)(server ->h_addr))->s_addr;

	//connect client to  the socket
	if (connect(sockfd,(const struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){// if not Succeeded
		perror("connect");
		free(request);
		exit(EXIT_FAILURE);

	}

	printf("HTTP request =\n%s\nLEN = %d\n", request, (int)strlen(request));
	//-------------------
	//write  the request to the server
	if((write(sockfd, request,strlen(request)))< 0) {
		perror("write");
		free(request);
		if(close(sockfd)<0){
			perror("close");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_FAILURE);
	}

	// read the answer
	char read_request[BUF_SIZE*4];
	int readed=1;
	int  all_readed=0;
	//--------------------------------------
	//while we have what to read
	while((readed=read(sockfd,read_request,BUF_SIZE*4))>0){// if error
		all_readed+=readed;
		//printf("%.*s",readed,read_request);
		write(1,read_request, readed);


	}
	if(readed<0){
		perror("read");
		free(request);
		if(close(sockfd)<0){
			perror("close");
			exit(EXIT_FAILURE);
		}
	}

	printf("\n Total received response bytes: %d\n",all_readed);

	//close the socket
	if(close(sockfd)<0){// if error
		perror("close");
		free(request);
		exit(EXIT_FAILURE);

	}
	//if all Successfully
	free(request);
	free(send_host);

	return 0;

}


//--------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//private method
void bad_usage(char* msg){
	fprintf(stderr,"%s",msg );
	exit(EXIT_FAILURE);



}
//------------------------------------------------------------------------------------------------------------------------
// private method- that compound the request we want to send
char* send_request( char* h_flag,char*d_flag,char* path,char* host, char *timebuf){
	int size_str=0;
	char* request;
	char* type="GET";
	char* modified="";
	if(strcmp(h_flag,"-h")==0){
		type="HEAD";
	}
	size_str+=strlen(type)+strlen("\r\n\r\nConnection: close");
	if(strcmp(d_flag,"-d")==0){
		modified=timebuf;
		size_str+=strlen("\r\nIf-Modified-Since: ")+strlen(timebuf);

	}

	size_str+=strlen(path)+strlen("  HTTP/1.1\r\nHOST: \r\n\r\n")+strlen(host);
	request=calloc(size_str+1,sizeof(char));//+1 for "/0"
	if(request==NULL){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	if(strcmp(d_flag,"-d")!=0){
		snprintf(request,size_str, "%s %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",type,path,host);
	}
	else{

		snprintf(request,size_str, "%s %s HTTP/1.1\r\nHost: %s\r\nIf-Modified-Since: %s\r\nConnection: close\r\n\r\n",type,path,host,modified);

		//printf("%s",request);
	}
	return request;

}

//-------------------------------------------------------------------------------------
//private method that calc the time interval
char* calc_time_interval (int *date){

	char* time1 = (char*)calloc (BUF_SIZE+1,sizeof(char));
	if(time1==NULL){
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	char time2[BUF_SIZE];
	int day=date[0];
	int hours= date[1];
	int minutes=date[2];


	now = time(NULL);
	now=now-((day*24*3600)+(hours*3600)+(minutes*60)); //where day, hour and min are the values //from the input
	strftime(time2, sizeof(time2), RFC1123FMT, gmtime(&now));
	//printf("%s\n", time2);
	if(strncpy(time1, time2,sizeof(time2))<0){
		exit(EXIT_FAILURE);
	}

	return time1;

}
//----------------------------------------------------------------------------------------------

