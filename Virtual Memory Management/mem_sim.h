#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//inclide for file
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define PAGE_SIZE 4
#define NUM_OF_PAGES 20
#define MEMORY_SIZE 24
# define SWAP_SIZE 80
#define NUM_OF_FRAME 6

typedef struct page_descriptor
{
unsigned int valid; //indicates if this page is mapped to a frame
unsigned int dirty; //indicates if this page was changed
unsigned int in_backing_store; //indicates if this page is in swap
unsigned int frame; //the number of a frame if in case it is page-mapped
} page_descriptor;

//-----------------------------------------------------------------------------------------------------------
typedef struct sim_database
{
page_descriptor * page_table; //pointer to page table
int swapfile_fd; //swap file fd
int program_fd; //executable file fd
char main_memory[MEMORY_SIZE];
int text_length;
int stack_size;
int correntFrame; // in which frame i insert data
int pageInFrame[ NUM_OF_FRAME];
}sim_database ;

//--------------------------------------------------------------------------------------------------------------------
sim_database  * init_system(char exe_file_name[], char swap_file_name[] , int text_size, int bss_heap_stack_size );
char load (sim_database * mem_sim , int address);
void store(sim_database * mem_sim , int address, char value);
int cheack_if_valid_address(int address);
void moveToSwap(sim_database * mem_sim);
void write_to_mainMemory(sim_database * mem_sim, int page, int offset, char* str);
void print_memory( sim_database * mem_sim);
void print_swap( sim_database * mem_sim);
void clear_system(sim_database * mem_sim);
