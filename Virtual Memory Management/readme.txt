

Description: 

In this exercise we will simulate a processor's approach to memory. 
 use the paging mechanism, which allows you to run a program when only a part It is in memory.
 Program memory (also known as virtual memory) is divided into pages that are brought to the main memory as needed
**We assume that this is a virtual memory of one program

The approaches to memory are implemented by 2 function: store and load.
1)store -   write to logical address
2)load - read from logical address

//---------------------------------------------------------------------------------------------------------------------------
The function:

sim_database * sim_db = init_system(char exe_file_name[], char swap_file_name[] , int text_size, int bss_heap_stack_size ):
	- Initializes the workspace

char load (sim_database * mem_sim , int address):
method that read from logical address
Receives an address to access for reading from the memmemory .
 makes sure that the echo of the  received address in question is in memory.
If Load function is invalid You must print an error message to stderr and continue simulating. 
If access is to the heap, but to a page Which has not yet been assigned, an error message will be sent to stderr and the simulation will continue as usual
if the address  find in  the memmory we will return it
if not we will bring it to the memmory for reading:
* the adrees is from the  heap area we will bring it from te swap file to the memmory
 (if it not there  mean we triying to acsses to un allocte addres)
*if the the addres from the code zone we will bring it, for reading, to the memmory from  executable file


void store(sim_database * mem_sim , int address, char value):
 method that write to logical address
Receives an address to access for writing. Also here, checks the page of the spoken address will be in memory.
 If the address is not in the memory space, or in a section with read-only privileges(code part ), will printed an error message to stderr to return from the function and continue the oprogram simulation.
 Accessing an unassigned page in the stacking section will be considered as an assignment of that page - a new page will be created in the page table and find him place in the memmory , 

 
int cheack_if_valid_address(int address):
method that chacks that   that thea  wanted addres is in the range of the  program address

void moveToSwap(sim_database * mem_sim):
method that checks when there is no  more place in the memmory  and the address located in the varible zoon  if need to move the frame that corently in the memmory   that we want to replace ( by FIFO order)  to the swap 


void write_to_mainMemory(sim_database * mem_sim, int page, int offset, char* str):
 method that write some page to the main memory

void print_memory( sim_database * mem_sim):
method that print  memmory's file data

void print_swap( sim_database * mem_sim):
method that print the swap file data 

void clear_system(sim_database * mem_sim):
 method that clear the  all alloceted memmory





 //------------------------------------------------------------------------------------------------------------------------------------


The files of the program:


makefile - this file compiling the file type .c  and the file the file type.h and creating "out" files 
README - explanation of the program
mem_sim.h - Contains statements about the functions, the sim_database struct and the page_descriptors struct
mem_sim.c - Contains the realization of functions 
main.c - Contain the main program that uses the functions
//-------------------------------------------------------------------------------------------------------

input:

	there are no input from the user.
	the main.c call to the function: init_system, clear_system, store and load 
	the input of these functions:
		init_system   - Getting the name of the swap file, name of executable file, the text size and the bass_heap_stack_size
		clear_system - Getting the sim_database
		store 	    - Getting the sim_database, address and char
		load            -  Getting the sim_database and address


output:	the output will be the print of: page table, swap, main memory according to the main
	and messeges error when necessary

//---------------------------------------------------------------------------------------------------------------------------------

compile: 

in order to compile in linux needed to use g++ command

worthwhile compile by using the file "makefile" to compile all files.

using the command : "make"

//---------------------------------------------------------------------------

To run the program

To run the program
command line: ./ex5q1 
