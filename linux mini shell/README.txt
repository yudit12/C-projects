

The program realizing a linux shell,
This shell print the user and the host in this format: [user]@[host]>
waiting for command, and try to run the command that the user write.
 our program willnot spurt all linux command such as cd but the most of them... 
 
 The function:
char* shell_format(): 
method shows the format shell

int count_spaces(char* str): 
 count num of space in the inpout
 
int split_by_qout(char** command, char* input): 
method that  split the input  by " "  and put it in temprory array 

int numOf_arguments(char** command, int counter): 
 count the realy num of arguments in the input
 
void final_command_arr(char** argv, char** arr, int counter):
method that create  the finall arr

**//void printArr(char** arr, int counter):
method that print  the array - cheaking for myself the input
**//




in the main  we will print the sell format  waiting to entred command 
and create process wiil notify as if the procces  couldnt created or faild 
and wiil end if the user entered "bye" 


The files of the program:

mini_shell.c -hold the main and the functions 
makefile - this file compiling the file type .c and creating an "out" file that contain the program.

input:
	the input need to be real command.
	(you can write not real command - the program print: "command not found").
	for exit write the word bye and press enter.

The output will be: 
	if the command not exist - the output will: "command not found".
	else the output will the out that we given from the orginal sheel
	for example - for the command: ls -l :
	 the orginal sheel display list of the files that there are in the folder
	and this program display the same.
		
		
compile: 
in order to compile in linux needed to use gcc command
worthwhile compile by using the file "makefile" to compile all files.
using the command : "make"


To run the program
command line: "./<the name of the file that the makefile created>"
(in this case "./ex1").



