
Exercise: EX3  - HTTP Server.

In this exercise we have 3 files:
	1) threadpool.c –    threadpool of thread implement the server jobs
	2)server.c - TCP server that  handle  all the server-client connections waiting  to client request to handle
	2) README - File that describes the files in the exercise.'
	
 functions: 
 in server.c
	1)char *get_mime_type- private method to deside the type of the file
	2)void handel_time-private method  that gives the correct time
	3)char* create_response- private method that create the apopprite response
	4)void search_sub_dir- private method that  that search for a sub dir if there is file of index.html if not reutrn the
									conntent of the dir
	5)char** split_request- private method that split the first line of the client  request and return 
	6)void send_error__request-  method that in case of error return the correct error response
	7)int dispatch_function(void *arg)- work method- that create connection bettwen the server and the client each thread will do it
								will also split the request and check it
	8)void handel_file- method that send  the correct  file content to the client
	9)void cheak_of_split_request- heck the first line of the request end return the appropriate error response if need to
	10)void bad_usage-method that display the usageerror
	11)int is_itDigit-that check that  number was realy insert in
	12)void handel_server-method that  create the sokcet and wait for request when client conect it handel is rerequest  and return appropriate e response
	13)char* create_table(char* path)- private method that display  all the file and dir  in given dir  as v table of name of file
 									size and the time of last Last-Modified
	14)char* content_dir_part_1(char* path)- method  that care of the of the content_dir  file
	
	in threadpool.c
		int init_threadpool(threadpool* pool, int num_threads)- private method helps to inizlize the initialize the threadpool structure

	

Remark:
	• in case there isnt  execute permission in any level will be send 403 error even if there is write read permission to the file
	• if there is no read permission also wil be send 403 error