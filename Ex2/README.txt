

Exercise: EX2 - HTTP client.

In this exercise we have 2 files:
	1) client.c – a program that implements HTTP client that constructs an HTTP request based on user’s command line input, sends the request to a Web server, receives the reply from the server, and displays the reply the message on screen.
	   (support only IPv4 connections.)  
	2) README - File that describes the files in the exercise.
 functions: 
	1)char* calc_time_interval -method that calc the given time interval to the need format.
	2) char* send_request- compound the request we want to send
	3) bad_usage(char* msg)- method that print the wanted error msg
	

Remark:
	• In case of of errors we exit(-1)
	• even if  port 80 wil be enterd (and not definedas default) it will not be seen in the reuqvest ater ports will be
	• time can be negtive 
	• if ther is no ':' in the url and there is numeber afther it the hos it will fail in the set of the hostByName - if there is no host like that 
