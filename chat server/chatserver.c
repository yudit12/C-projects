#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <ctype.h>
#include <netdb.h>//contain method gethostbyname
#include <netinet/in.h>
#include <sys/socket.h>
#include "slist.h"
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
//------------------------------------
#define  null NULL
#define TRUE 1
#define MAX_TO_READ 4096
#define CAL_SYS 1
#define USER_MIS 0
//==================================================================
//methods
void bad_usage(char* msg,int type);
int is_itDigit(const char* argv);
int open_connection(int port, struct sockaddr_in * address);
char* read_function(int fd, slist_t *slist);
void  write_function(int fd, char* msg, slist_t *slist);
void update_max_fd(int* max_fd, slist_t *list);
int* cast_int_to_intPointer(int new_socket );
typedef void (*sighandler_t)(int);
sighandler_t signal(int,sighandler_t);
void handler (int signum);


//-------------------
slist_t* slist;
int main_fd;
//===================================================================
//main
int main(int argc, char **argv) {

	if (argc !=2){
		bad_usage("bad usage you should write ./server <port> ",USER_MIS);
	}
	int port=is_itDigit(argv[1]);

	struct sockaddr_in address;
	 main_fd=open_connection(port,& address );

	int addrlen= sizeof(address);
	int max_fd;
	fd_set read_fds;
	fd_set write_fds;
	int new_socket;
	slist=calloc(1,sizeof(slist_t));
	if(slist==null){
		bad_usage("calloc",CAL_SYS);
	}
	signal(SIGINT,handler);
	slist_init(slist);

	while(TRUE)
	{
		//preparing  for the select - what we going to send
		FD_ZERO(&read_fds);// init func
		FD_ZERO(&write_fds);

		slist_node_t *temp = slist->head;
		while (temp != null)
		{
			FD_SET(*((int*)temp->data), &read_fds);// add individuals fds to set
			FD_SET(*((int*)temp->data), &write_fds);// add individuals fds to set
			temp = temp->next;
		}

		FD_SET(main_fd, &read_fds);
		max_fd = main_fd; // init of max-fd
		update_max_fd(&max_fd, slist);
		int activity = select(max_fd+ 1 , &read_fds , &write_fds , NULL , NULL);
		if (activity < 0)
		{
			bad_usage("select",CAL_SYS);
		}
		//		if(activity == 0) {
		//			continue;
		//		}
		// run on all slist

		if (FD_ISSET(main_fd, &read_fds))
		{
			new_socket = accept(main_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			if (new_socket < 0 && slist->size==0)//no connection  was formed- notzhar
			{
				bad_usage("accept", CAL_SYS);
				//				continue;
			}
			//if(new_socket > 0) {
			int *temp_fd = (int*)calloc(1, sizeof(int));
			*temp_fd = new_socket;
			slist_append(slist, (void*)temp_fd);
			//free(temp_fd);

			//}
		}

		slist_node_t *ptr;
		for(ptr=slist_head(slist); ptr!=null;)
		{
			slist_node_t* next = slist_next(ptr);
			// testing if fd is ready to  write to us ( to the server)
			int temp_fd = *((int*) ptr->data);
			char* msg = null;
			if (FD_ISSET(temp_fd , &read_fds))
			{
				printf("server is ready to read from socket <%d>\n",temp_fd);
				msg = read_function(temp_fd,slist);
				if (msg == null)
				{
					FD_CLR(temp_fd, &read_fds);
					FD_CLR(temp_fd, &write_fds);
					ptr = next;
					continue;
				}
				char *temp_chars = (char*)calloc(MAX_TO_READ, sizeof(char));
				strncpy(temp_chars, msg, MAX_TO_READ);
				sprintf(msg, "Guest %d: %s", temp_fd, temp_chars);
				free(temp_chars);

				slist_node_t *ptr2;
				for(ptr2=slist_head(slist); ptr2!=null && msg != null && strlen(msg) > 0 ;ptr2=slist_next(ptr2))
				{
					int temp_fd = *((int*) ptr2->data);
					if (FD_ISSET(temp_fd , &write_fds))
					{
						printf("server is ready to write to socket <%d>\n",temp_fd);
						write_function(temp_fd, msg, slist);

					}
				}
				free(msg);
			}

		ptr=next;
		}
	}
	return 0;



}

//====================================================================
// handle in case of forsing exit- control c 
void handler (int signum){

	slist_destroy(slist,SLIST_FREE_DATA);
	free(slist);
	close(main_fd);
	exit(EXIT_SUCCESS);


}

//--------------------------------------
// private method that  create the sokcet and handle the open connection of the server

int open_connection(int port, struct sockaddr_in * srv){

	int fd;// main pipe
	if((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		bad_usage("socket",CAL_SYS);
	}

	// create the socket

	srv->sin_family = AF_INET;  //use the Internet addr family

	srv->sin_port = htons(port); // bind socket ‘fd’ to port 80

	// bind: a client may connect to any of server addresses
	srv->sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(fd, (struct sockaddr*) srv, sizeof(*srv)) < 0) {
		bad_usage("bind",CAL_SYS);

	}

	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK); // make the socket non-blocking

	// the server ready to lisen for

	if(listen(fd, 0) < 0) {// 0 - how many  client as want can connent to server
		bad_usage("listen",CAL_SYS);

	}
	// there is no clos(fd) becasuse  as many as wantend client can to connect
	return fd;
}

//-----------------------------------------------------------------------
// function  in  the server read from the client
// the funch will  be call only if there is  data to be readed
char* read_function(int fd, slist_t *slist){
	// read the request;
	int readed = 0;

	char *msg = (char*)calloc(MAX_TO_READ, sizeof(char));
	readed=recv(fd,msg,MAX_TO_READ-1,MSG_PEEK);

	if(readed<0)
		bad_usage("read",CAL_SYS);
	if (readed == 0)
	{
		remove_node(slist, (void*)&fd);
		if(close(fd)<0)
			bad_usage("close",CAL_SYS);
		free(msg);
		return null;
	}
	int length = strstr(msg, "\r\n")-msg+2;
	if ( (readed = recv(fd, msg, length, 0)) < 0 )
		bad_usage("close",CAL_SYS);
	msg[readed] = '\0';
	return msg;
}
//------------------------------------------------------------
//function  in the server write  the  to all client the msg
void  write_function(int fd, char* msg, slist_t *slist){
	int writen;
	writen=send(fd, msg, strlen(msg), 0);
	if(writen < 0){
		remove_node(slist, (void*)&fd);
		close(fd);
	}
}


//------------------------------------
//private method that check that  number was really insert in
int is_itDigit(const char* argv){
	int i;
	for(i=0;i<strlen(argv);i++){
		if(isdigit(argv[i])==0){
			bad_usage("not a valid digit",USER_MIS);
		}
	}
	return atoi(argv);

}

//---------------------------------------------------
//private method print  error cheacks if it call system error or  user error  and any way exit
void bad_usage(char* msg,int type ){
	if(type==USER_MIS){
		fprintf(stderr,"%s",msg);

	}
	else{
		perror(msg);
	}
	exit(EXIT_FAILURE);

}
//---------------------------------------------------------
// private method that update_max_fd value  for sending it to the select function
void update_max_fd(int* max_fd, slist_t *list){
	slist_node_t *ptr;
	for(ptr=slist_head(list); ptr!=null;ptr=slist_next(ptr)){
		int fd =*((int*)slist_data(ptr));
		if(fd>*max_fd){
			*max_fd=fd;
		}
	}

}
//-------------------------------------------------
//private method  that convert int  to a pointer 
int* cast_int_to_intPointer(int new_socket ){
	int *new_fd= calloc(1 ,sizeof(new_fd));
	if(new_fd==null){
		bad_usage("calloc",CAL_SYS);
	}
	*new_fd=new_socket;
	return new_fd;
}
//------------------------------------------------------
