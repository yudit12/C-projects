
Exercise: EX4 - Chat Server with select.

In this exercise we have 5 files:
	1)chatserver.c - a program that implements tcp server that  can read /write msg  from/to  multiply clients 
	2) slist.c -  Implemention of the link list which can contain  any type of data in this exercise hold the activy  fds
	3)slist.h -   Declares the functions that we use in "slist.c" and nedeed structures.
	4) Makefile – helps us to organize code compilation.
	5) readme - File that describes the files in the exercise.
	
	
 functions:
	•in slist.c :
	used all function from ex1 and in  addion add a new func of removeing node  frome list (add also the signature  to the slist.h)
	
	•in chatserver.c:
	1)open_connection -  method that  create the sokcet and handle the open connection of the server
	2)read_function- function  in  the server read from the client and return msg 
	3)write_function- function  in  the server write  the  to all client the msg
	4)is_itDigit- method that check that  number was really insert in
	5)void bad_usage - private method print  error cheacks if it call system error or  user error  and any way exit
	6)update_max_fd- method that update_max_fd value  for sending it to the select function
	7)cast_int_to_intPointer- method  that convert int  to a pointer 
	8)void handler (int signum)-handle in case of forsing exit- control c 
	

Remark:
	• In case of of errors we exit(-1)
	• we  exit the server  with control c 
	•important use also slist.c and slist .h
	
	in addion I add my makefile
	for compile the program write in the terminal- make
	for runnig the program - ./ server [enter port]
	
	
