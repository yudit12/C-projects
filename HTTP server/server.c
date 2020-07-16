#include "threadpool.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/socket.h>

#define STATUS302 "302"
#define PHRASE302 "Found"
#define SNTNCE302 "Directories must end with a slash."
#define STATUS400 "400"
#define PHRASE400 "Bad Request"
#define SNTNCE400 "Bad Request."
#define STATUS403 "403"
#define PHRASE403 "Forbidden"
#define SNTNCE403 "Access denied."
#define STATUS404 "404"
#define PHRASE404 "Not Found"
#define SNTNCE404 "File not found."
#define STATUS500 "500"
#define PHRASE500 "Internal Server Error"
#define SNTNCE500 "Some server side error."
#define STATUS501 "501"
#define PHRASE501 "Not supported"
#define SNTNCE501 "Method is not supported."

#define PRINT_FILE "print file"
#define ERROR -1
#define OK_AND_PRINT 2
#define OK 1
#define ERROR500 500
#define EMPTY 0
#define QLEN 5	//maximum number of client connections
#define BUFLEN 4000
#define BUFLENTIME 128
#define NUM_ARGC_REQUIRED 4
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"

#define DIR_CONTANT_1 "<HTML>\r\n<HEAD><TITLE>Index of "
#define DIR_CONTANT_2 "</TITLE></HEAD>\r\n\r\n<BODY>\r\n<H4>Index of "
#define DIR_CONTANT_3 "</H4>\r\n\r\n<table CELLSPACING=8>\r\n<tr><th>Name</th><th>Last Modified</th><th>Size</th></tr>\r\n\r\n\r\n"
#define DIR_CONTANT_4 "./"
#define DIR_CONTANT_5 "</table>\r\n\r\n<HR>\r\n\r\n<ADDRESS>webserver/1.1</ADDRESS>\r\n\r\n</BODY></HTML>\r\n"
#define DIR_TABLE_1 "<tr>\r\n<td><A HREF=\""
#define DIR_TABLE_2 "\">"
#define DIR_TABLE_3 "</A></td><td>"
#define DIR_TABLE_4 "</td>\r\n<td>"
#define DIR_TABLE_5 "</td>\r\n</tr>"
#define SIZE_LEN_IN_TABLE 500
#define ERROR_500_1 "HTTP/1.1 500 Internal Server Error\r\nServer: webserver/1.1\r\nDate: "
#define ERROR_500_2 "Content-Type: text/html\r\nContent-Length: 144\r\nConnection: close\r\n\r\n"
#define ERROR_500_3 "<HTML><HEAD><TITLE>500 Internal Server Error</TITLE></HEAD>\r\n<BODY><H4>500 Internal Server Error</H4>\r\nSome server side error.\r\n</BODY></HTML>\r\n"
#define START_200_OK "HTTP/1.1 200 OK\r\nServer: webserver/1.1\r\nDate: "
#define CONECT_TYPE "\r\nContent-Type: "
#define CONECT_LEN "\r\nContent-Length: "
#define CONECT_MODIFIED "\r\nLast-Modified: "
#define CONECT_CLOSE "Connection: close\r\n\r\n"
#define MSG_ERROR_1 "<HTML><HEAD><TITLE>"
#define SPACE " "
#define MSG_ERROR_2 "</TITLE></HEAD>\r\n<BODY><H4>"
#define MSG_ERROR_3 "</H4>\r\n"
#define ENTER "\r\n"
#define MSG_ERROR_5 "</BODY></HTML>\r\n"	

#define CREATE_CONECT_1 "HTTP/1.1 "
#define CREATE_CONECT_2 "\r\nServer: webserver/1.1\r\nDate: "
#define CREATE_CONECT_3 "\r\nLocation: "
#define CREATE_CONECT_4 "/"
#define CREATE_CONECT_5 "\r\nContent-Type: text/html\r\nContent-Length: "
#define CREATE_CONECT_6 "\r\nConnection: close\r\n\r\n"


typedef struct server_support {
 	int flag;	
	char timebuf[BUFLENTIME]; 
	char buffer[BUFLEN];
} serverSupport;
//------------------------------------------------------------------------------------
void errorMsg(char *msg);
void error500(serverSupport* sup,char*  timebuf,int *newsockfd);
void errorInput(char *msg);
char *get_mime_type(char *name);
int strToInteger(char *str);
int isDirectory(const char *path);
void readFromFile(char *filename, int *newsockfd);
void read_dir_content(int *numbytes,char *filename,int *newsockfd,char* path);

char* handle_error(int *numbyte,char* status,char* phrase,char* sentence);

void handle_input(serverSupport* sup,int *newsockfd);
char* create_conectOk(int len,char* timebuf,char* type,int *newsockfd,char* path);
char* create_conect(int *numbytes,char* status,char* phrase,int len,char* sentence,char* timebuf,char* option,int *newsockfd);
int numDigits(int num);
int readInput(void* newsockfdarr);
int chackIfPermissions(char *path);

//------------------------------------------------------------------------------------

int main(int argc, char *argv[]) { 
	int sockfd,i;  
	struct sockaddr_in serv_addr; 
	struct sockaddr cli_addr; 
	
	socklen_t client = sizeof(cli_addr); 

	if (argc != NUM_ARGC_REQUIRED ) { 
		 errorInput("Usage: server <port> <pool-size> <max-number-of-request>\n");	
	} 
	sockfd = socket(PF_INET,SOCK_STREAM,0); 
	if (sockfd < 0)  
		errorMsg("socket"); 
	serv_addr.sin_family = AF_INET;  
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	serv_addr.sin_port = htons(atoi(argv[1]));   
	

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)                 
		errorMsg("binding");  
  
	if(listen(sockfd,QLEN)<0)
		errorMsg("listen");
	
	int pool_size=atoi(argv[2]);
	threadpool* threadpl=create_threadpool(pool_size);
	
	
	int max_request=strToInteger(argv[3]);
	
	int newsockfdarr[max_request];


		
	for(i=0;i<max_request;i++){
		


		newsockfdarr[i]= accept(sockfd, (struct sockaddr *)&cli_addr, &client);
		if (newsockfdarr[i] < 0)                 
			errorMsg("accept");


		dispatch(threadpl,readInput,(void*)&newsockfdarr[i]);

		
	}
	
	destroy_threadpool(threadpl);
	
	close(sockfd);

	return 0;
}
/****************************************************
***********f u n c t i o n   a r e a*****************
*****************************************************/
int readInput(void* newsockfdarr) {// The function to which the threads arrive, read the buffer input, and call to the function that handle input

	int* newsockfd =(int*) newsockfdarr;
	
	int nbytes;
	serverSupport* sup = (serverSupport *) malloc(sizeof(serverSupport));
	sup->flag=EMPTY;
	if(sup==NULL){
		error500(sup,sup->timebuf,newsockfd);
		free(sup);
		close(*newsockfd);
		return 0;
	}

	if( (nbytes = read((*newsockfd), sup->buffer, sizeof(sup->buffer))) < 0){////check &

		if(close(*newsockfd) < 0){
			free(sup);
			error500(sup,sup->timebuf,newsockfd);
			return 1;
		}
		free(sup);	
		perror("read");
		error500(sup,sup->timebuf,newsockfd);
		
	}
	
	if(nbytes>0){
		time_t now; 
		now = time(NULL);  
		strftime(sup->timebuf, sizeof(sup->timebuf), RFC1123FMT, gmtime(&now));
	
		handle_input(sup,newsockfd);
	}

	free(sup);
	close(*newsockfd);
	
	return 0;
}




//-------------------------------------------------
void errorMsg(char *msg) {  //exit with messege

	perror(msg);      
	exit(1); 
	
}
//-------------------------------------------------- 
void errorInput(char *msg) { //input error
	
	fprintf(stderr, "%s", msg );   
	exit(1);
}
//--------------------------------------------------
void error500(serverSupport* sup,char* timebuf,int* newsockfd) {
//error 500 Treated separately (if this function used: create_conect and handle_error, the error 500 could not be performed in these functions)

	int mallocLen=strlen(ERROR_500_1)+strlen(ERROR_500_2)+strlen(ERROR_500_3)+strlen(timebuf)+1;
	char* msg=(char*)malloc(sizeof(char)*(mallocLen));//change
	strcpy(msg,ERROR_500_1);
	strcat(msg,timebuf);
	strcat(msg,ERROR_500_2);
	strcat(msg,ERROR_500_3);

	sup->flag=ERROR500;
	write(*newsockfd, msg,mallocLen);
	free(msg);


}
//--------------------------------------------------
void handle_input(serverSupport* sup,int* newsockfd){//Disassembles the request and decides if the request is valid or need to out error msg

	char* path;
	char* pathAndIndex;
 	char* type;
	char* msg ;	
	char* msg2 =NULL;
	char* msgConect;

	
	int nbytes,i;
	unsigned long len;
	int lastChar;
	char *status,*phrase,*sentence,*option;
	char *nameFile;
	struct stat info;
	struct stat statbuf; 

	if(strstr(sup->buffer, "HTTP/1.1")==NULL && strstr(sup->buffer, "HTTP/1.0")==NULL){
		status=STATUS400;
		phrase=PHRASE400;
		sentence=SNTNCE400;
		sup->flag=ERROR;
		len=113;
		option="";				
	}	
	if(strncmp(sup->buffer, "GET",3)!=0 && sup->flag==EMPTY){ 
		status=STATUS501;
		phrase=PHRASE501;
		sentence=SNTNCE501;
		sup->flag=ERROR;
		len=129;
		option="";
	}
		
	//get path:
	path=strtok(sup->buffer,"/");
	path=strtok(NULL," ");// in the case that the path is empty - he think that the path is http/1.1 or http/1.0

	
	if(path==NULL || strncmp(path,"HTTP/1.1",8)==0 || strncmp(path,"HTTP/1.0",8)==0 ){
		path="/";

	}
	
	if((strcmp(path,"")!=0) && (stat( path, &info ) != 0) && (sup->flag==EMPTY)){
		status=STATUS404;
		phrase=PHRASE404;
		sentence=SNTNCE404;
		sup->flag=ERROR;
		len=112;
		option="";	
	}
	if(strcmp(path,"")!=0)
		lastChar=strlen(path)-1;
	else
		lastChar=0;
	
		
		
	pathAndIndex=(char*)malloc(sizeof(char)*(10+1+(lastChar+2)));//10 length of the string: "index.html"
	if(pathAndIndex==NULL){
		error500(sup,sup->timebuf,newsockfd);
	}	
	
	strcpy(pathAndIndex,path);
	strcat(pathAndIndex,"index.html");
	nameFile=(char*)malloc(sizeof(char)*(strlen(pathAndIndex) + 1));
	if(nameFile==NULL){
		error500(sup,sup->timebuf,newsockfd);
	}			
	if(((strcmp(path,"")==0) || isDirectory(path)!=0) && (sup->flag==EMPTY)){
		if(path[lastChar]=='/'){

			
			if (stat (pathAndIndex, &statbuf) == 0){
				if(chackIfPermissions(pathAndIndex)==1){//if exist file index.html


					sup->flag=OK_AND_PRINT;

					strcpy(nameFile,pathAndIndex);
					stat(path,&statbuf);
			
					len=statbuf.st_size;	
                                	type="text/html"; 
				}
				else{//no permissions

					status=STATUS403;
					phrase=PHRASE403;
					sentence=SNTNCE403;
					sup->flag=ERROR;
					len=111;
					option="";
				}
		
			}
			else
			{	
				if(chackIfPermissions(path)==1){

					sup->flag=OK;

					
					
					strncpy(nameFile,path,1+(lastChar+1));
					
					
					struct dirent **namelist;
					
					
					int lenDirName=strlen(nameFile)+2+1;// +1 it is just for /0 (2 for the ./)
					char* dirName=(char*)malloc(sizeof(char)*(lenDirName) + 1);

					strcpy(dirName,DIR_CONTANT_4);
					strcat(dirName,nameFile);
					
					int n = scandir(dirName, &namelist, NULL, alphasort);
	
					len=n*500+strlen(path)*2+strlen(DIR_CONTANT_1)+strlen(DIR_CONTANT_2)+strlen(DIR_CONTANT_3)+strlen(DIR_CONTANT_4)+strlen(DIR_CONTANT_5);
				
					type="text/html";
					option ="text/html";

					for(i=0;i<n;i++)
						free(namelist[i]);
					free(namelist);
					free(dirName);

				}
				else{//no permissions

				
					status=STATUS403;
					phrase=PHRASE403;
					sentence=SNTNCE403;
					sup->flag=ERROR;
					len=111;
					option="";
				}	
			}

			

		}
		
		else{
		
			status=STATUS302;
			phrase=PHRASE302;
			sentence=SNTNCE302;
			sup->flag=ERROR;
			len=123;
			option=path;		
		}
		 
	}
	free(pathAndIndex);
	if(sup->flag==EMPTY){//it is a file

		type=get_mime_type(path);
		stat(path,&statbuf);
		if( (S_ISREG(statbuf.st_mode)!=0) && chackIfPermissions(path)==1){
   
			sup->flag=OK_AND_PRINT;
			strcpy(nameFile,path);
			len=statbuf.st_size;
		}
		else{

			status=STATUS403;
			phrase=PHRASE403;
			sentence=SNTNCE403;
			sup->flag=ERROR;
			len=111;
			option="";	


		}
	}

	
	if(sup->flag==ERROR){
		msgConect=create_conect(&nbytes,status,phrase,len,sentence,sup->timebuf,option,newsockfd);
		msg2 = handle_error(&nbytes,status,phrase,sentence); 


		msg= (char*)malloc(sizeof(char)*(strlen(msgConect)+strlen(msg2) + 1));

		strcpy(msg, msgConect);

		strcat(msg, msg2);

		
		if((nbytes=write(*newsockfd, msg,strlen(msg)))< 0){

			error500(sup,sup->timebuf,newsockfd);
		}
		
				
		free(msg);
		free(msg2);
		free(msgConect);	

			

	}	
		
	else if((sup->flag==OK_AND_PRINT)||(sup->flag==OK)){
	
		msgConect=create_conectOk(len,sup->timebuf,type,newsockfd,path);
		if((nbytes=write(*newsockfd, msgConect,strlen(msgConect)))< 0){		
			error500(sup,sup->timebuf,newsockfd);
		}

		free(msgConect);		

		if(sup->flag==OK_AND_PRINT){

			readFromFile(nameFile,newsockfd);
		}
		else if(sup->flag==OK){

			read_dir_content(&nbytes,nameFile,newsockfd,path);

		}
		
	
	}
free(nameFile);
	
		

}
//--------------------------------------------------
int numDigits(int num){//Count the number of digits

	int len=0;
	while(num>0){
		num/=10;
		len++;
	}
	
	return len;

}


//--------------------------------------------------
char* create_conectOk(int len,char* timebuf,char* type,int *newsockfd,char* path){//Creates conect of valid request

	int lenMalloc;
	struct stat attr;
    	stat(path, &attr);
    	char* time=ctime(&attr.st_mtime);


	lenMalloc=(strlen(START_200_OK)+strlen(timebuf)+strlen(CONECT_TYPE)+strlen(CONECT_LEN)+numDigits(len)+strlen(CONECT_MODIFIED)+strlen(CONECT_CLOSE)+strlen(time)+1);	
	if(type!=NULL)
		lenMalloc+=strlen(type);
	char* conectOkMsg = (char*)malloc(sizeof(char)*lenMalloc +1);

	memset(conectOkMsg,0,lenMalloc);
	strcpy(conectOkMsg, START_200_OK); 
	strcat(conectOkMsg, timebuf);
	if(type!=NULL){
		strcat(conectOkMsg, CONECT_TYPE);
		strcat(conectOkMsg, type);
	}

	strcat(conectOkMsg, CONECT_LEN);

	int sizeLen= numDigits(len);
	char* strLen = (char*)malloc(sizeof(char)*sizeLen+2);

		
	sprintf(strLen, "%d", len);

	strcat(conectOkMsg, strLen);
	strcat(conectOkMsg, CONECT_MODIFIED);
	strcat(conectOkMsg, time);
	strcat(conectOkMsg, CONECT_CLOSE);
	free(strLen);

	
	return conectOkMsg;

}
//------------------------------------------------------------------------------------------------------------------------
char* create_conect(int *numbytes,char* status,char* phrase,int len,char* sentence,char* timebuf,char* option,int *newsockfd){//Creates conect of no valid request

	int sizeLen= numDigits(len);

	int lenMalloc=(strlen(CREATE_CONECT_1)+strlen(status)+strlen(SPACE)+strlen(phrase)+strlen(CREATE_CONECT_2));
	lenMalloc+=(strlen(timebuf)+strlen(CREATE_CONECT_5)+numDigits(len)+strlen(CREATE_CONECT_6)+1);	
	if(strcmp(status,STATUS302)==0){
		lenMalloc+=(strlen(CREATE_CONECT_3)+strlen(option)+strlen(CREATE_CONECT_4));
	}
		         				
	char* msg = (char*)malloc(sizeof(char)*(lenMalloc));

						

	char* strLen = (char*)malloc(sizeof(char)*sizeLen+1);//need do a function that check how mach digit in the len!!

	sprintf(strLen, "%d", len);
		
	strcpy(msg, CREATE_CONECT_1);

	strcat(msg, status);
	strcat(msg, SPACE);
	strcat(msg, phrase);


	strcat(msg, CREATE_CONECT_2);


	strcat(msg,timebuf);

	if(strcmp(status,STATUS302)==0){
		strcat(msg,CREATE_CONECT_3);
		strcat(msg,option);
		strcat(msg,CREATE_CONECT_4);
	}
	strcat(msg,CREATE_CONECT_5);

	strcat(msg,strLen);
	strcat(msg,CREATE_CONECT_6);

	 

	(*numbytes)=strlen(msg);
	
	
	free(strLen); 


	return msg;
}



//---------------------------------------------------
char* handle_error(int *numbytes,char* status,char* phrase,char* sentence){//Creates html for error msg


      int lenMalloc=(strlen(MSG_ERROR_1)+strlen(status)*2+strlen(SPACE)*2+strlen(phrase)*2+strlen(MSG_ERROR_2)+strlen(MSG_ERROR_3)+strlen(sentence)+strlen(ENTER)+strlen(MSG_ERROR_5)+1);
		
		        

	char* msg_error = (char*)malloc(sizeof(char)*(lenMalloc + 1));



		

	strcpy(msg_error,MSG_ERROR_1);
	
	strcat(msg_error, status);
	strcat(msg_error, SPACE);
	strcat(msg_error, phrase);
	strcat(msg_error,MSG_ERROR_2);
	strcat(msg_error, status);
	strcat(msg_error, SPACE);
	strcat(msg_error, phrase);
	strcat(msg_error,MSG_ERROR_3);
	strcat(msg_error,sentence);
	strcat(msg_error,ENTER);
	strcat(msg_error,MSG_ERROR_5);

		
		
	(*numbytes)=strlen(msg_error);



	return msg_error;
}
//--------------------------------------------------------------------------------
int chackIfPermissions(char *path){//check permissions- return 1 if there is permissions in path

	if(strcmp(path,"/")==0){
		return 1;
	}
	
	int sizePath=strlen(path);
	char* workWithSlash=(char*)malloc(sizeof(char)*(sizePath)+1);
	char* subPath=(char*)malloc(sizeof(char)*(sizePath)+1);
	char *slash;
	struct stat bufst;
	strncpy(subPath,path,sizePath);
	subPath[sizePath]='\0';
	stat(subPath, &bufst);

	if(!(bufst.st_mode & S_IFDIR)){ //file

		if(!(bufst.st_mode & S_IRUSR) || !(bufst.st_mode & S_IRGRP) || !(bufst.st_mode & S_IROTH)){
			free(subPath);
			free(workWithSlash);
			return 0;
		}
			
	}

	slash = strtok(subPath, "/");
	
	if(slash == NULL) {
		free(subPath);
		free(workWithSlash);
		return 1;
	}
	strcpy(workWithSlash,slash);
	while( slash != NULL ){	
		stat(workWithSlash, &bufst);
		if(bufst.st_mode & S_IFDIR){	
			if(!(bufst.st_mode & S_IXUSR) ||!(bufst.st_mode & S_IXOTH) ||!(bufst.st_mode & S_IXGRP)){
				free(subPath);
				free(workWithSlash);
				return 0;
			}
					
			else if(!(bufst.st_mode & S_IRUSR) ||!(bufst.st_mode & S_IROTH) ||!(bufst.st_mode & S_IRGRP)){
				free(subPath);
				free(workWithSlash);
				return 0;
			}
			
		}//dir
		slash = strtok(NULL, "/");
		if(slash != NULL){
			strcat(workWithSlash,"/");
			strcat(workWithSlash,slash);
		}
	}
	free(subPath);
	free(workWithSlash);
	return 1;
}
//--------------------------------------------------
char *get_mime_type(char *name) { //Checking the file type

	char *ext = strrchr(name, '.');  
	if (!ext) return NULL;  
	if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";  
	if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";  
	if (strcmp(ext, ".gif") == 0) return "image/gif";  
	if (strcmp(ext, ".png") == 0) return "image/png";  
	if (strcmp(ext, ".css") == 0) return "text/css";  
	if (strcmp(ext, ".au") == 0) return "audio/basic";  
	if (strcmp(ext, ".wav") == 0) return "audio/wav";  
	if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";  
	if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0) return "video/mpeg";  
	if (strcmp(ext, ".mp3") == 0) return "audio/mpeg";  
	return NULL; 

}  
//------------------------------------------------------------
int strToInteger(char *str){//converter str to integer

	int i=0;

	for(;i<strlen(str);i++){
			
  		if( !(isdigit(str[i])) ){

     			errorInput("wrong input\n"); //before the accept there for no 500 error
      		}

   	}

   	return atoi(str);

}
//----------------------------------------------------------------

int isDirectory(const char *path) {//check if path is a Directory, return non-zero if yes, else return 0

	struct stat statbuf;
   	if (stat(path, &statbuf) != 0)
       		return 0;

   	return S_ISDIR(statbuf.st_mode);//This macro returns non-zero if the file is a directory

}//----------------------------------------------------------------
void read_dir_content(int *numbytes,char *filename,int *newsockfd,char* path){//print the dir Items 


	int lenDirName=strlen(filename)+2+1;
	char* dirName=(char*)malloc(sizeof(char)*(lenDirName));

	memset(dirName,0,lenDirName);
	char* strLen; 
	struct dirent **namelist;
	struct stat statbuf;
	int n;
	if(strcmp(path,"/")!=0)
		strcpy(dirName,DIR_CONTANT_4);
	else
		strcpy(dirName,".");
	strcat(dirName,filename);
	

	
	n = scandir(dirName, &namelist, NULL, alphasort);

	int lenMalloc=(strlen(DIR_CONTANT_1)+strlen(filename)+strlen(DIR_CONTANT_2)+strlen(filename)+strlen(DIR_CONTANT_3));
	lenMalloc+=((SIZE_LEN_IN_TABLE*n)+strlen(DIR_CONTANT_5));
	
	char*  msg_dir= (char*)malloc(sizeof(char)*(lenMalloc));


  
	strcpy(msg_dir,DIR_CONTANT_1);
	strcat(msg_dir,dirName);//filename
	strcat(msg_dir,DIR_CONTANT_2);
	strcat(msg_dir,dirName);//filename
	strcat(msg_dir,DIR_CONTANT_3);

	

	 
	
	if (n == -1) {
		perror("scandir");
		exit(EXIT_FAILURE);////exit500
	}

	

	int len;
	unsigned long size;
	while (n--) {
		len=strlen(dirName)+strlen(namelist[n]->d_name) + 4;
		char pathAndName[len];

		strcpy(pathAndName,dirName);
		
		strcat(pathAndName,namelist[n]->d_name);


		strcat(msg_dir,DIR_TABLE_1);

		strcat(msg_dir,namelist[n]->d_name);

		
		if(isDirectory(pathAndName)!=0){

			strcat(msg_dir,"/");
		}
					
		
		strcat(msg_dir,DIR_TABLE_2);
		strcat(msg_dir,namelist[n]->d_name);
			
		strcat(msg_dir,DIR_TABLE_3);
		
		stat(pathAndName,&statbuf);

		char* timebuf=ctime((&statbuf.st_mtime));
		strcat(msg_dir,timebuf);
		strcat(msg_dir,DIR_TABLE_4);
		if(isDirectory(pathAndName)==0){

			
			size = statbuf.st_size;
			strLen = (char*)malloc(sizeof(char)*numDigits(size)+2);

			sprintf(strLen, "%lu", size);
			
			
			strcat(msg_dir,strLen);
			strcat(msg_dir,DIR_TABLE_5);
		
			
			free(strLen);
			
		}
		free(namelist[n]);
	
	}

          
	strcat(msg_dir,DIR_CONTANT_5);
          
	free(dirName);
	free(namelist); 
	
	if(write(*newsockfd, msg_dir,strlen(msg_dir))< 0)
		errorMsg("write");

	free(msg_dir);

}
//----------------------------------------------------------------

void readFromFile(char *filename, int *newsockfd){//read from file and write to the newsockfd


	FILE *fd;
	size_t size =1024;
  	unsigned char buf[size];
  	if ((fd = fopen(filename,"rb")) < 0)
    		errorMsg("open");
    	fseek(fd,0,SEEK_SET);
 
	int numByte=1;


	
	
	
	while(numByte>0){
		if( (numByte = fread (buf,1,sizeof(buf), fd))< 0){

			errorMsg("read");
		}
		
		if(numByte>0 && buf != NULL){
			
			if(write(*newsockfd, buf,numByte) < 0){ 
	
				errorMsg("write");
			}
			bzero(buf,sizeof(buf));


		}

	}



		 fclose(fd);


}



