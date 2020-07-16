#include "mem_sim.h"
//-------------------------------------------------------------------------------------------------------------------------------------
// method that inislize workspace  needded file and   data 
sim_database  * init_system(char exe_file_name[], char swap_file_name[] , int text_size, int bss_heap_stack_size ){

sim_database * sim_db=(sim_database *)malloc(sizeof(sim_database));
 
    if(sim_db==NULL){
        perror("null pointer exception\n");
        exit(1);
    }
  
   
sim_db->program_fd=open ( exe_file_name, O_RDONLY,0); //zero becase we are just open exsit file and we just want read

// ceack if  file ssecsefly open
if (sim_db->program_fd==-1){
	perror("faild to open");
	free(sim_db);
	exit(1);

}

sim_db->swapfile_fd=open (swap_file_name, O_RDWR | O_CREAT | O_TRUNC,0600);
if (sim_db->swapfile_fd==-1){
	perror("faild to open");
	close(sim_db->program_fd);// close file  that secceded to open before exit the program for relesing memmory.
	free(sim_db);
	exit(1);

}

int i=0;
//inlisize swap file to zero
char s[SWAP_SIZE];
for(i = 0; i < SWAP_SIZE; i++)
{
	s[i] = '0';
}

lseek(sim_db->swapfile_fd, 0, SEEK_SET);
write(sim_db->swapfile_fd, s, SWAP_SIZE);

sim_db->text_length=text_size;
sim_db->stack_size= bss_heap_stack_size; 
sim_db->correntFrame=0;

// int array that hold which page in which frame
for( i=0;i< NUM_OF_FRAME;i++){
	sim_db->pageInFrame[i]=0;

}

 
 // init  ram memmory

for( i=0;i<MEMORY_SIZE;i++){
	sim_db->main_memory[i]='0';

}

// init   pages date
sim_db->page_table=(page_descriptor*)malloc(sizeof(page_descriptor)*NUM_OF_PAGES); 

if(sim_db->page_table==NULL){
        perror("null pointer exception\n");
        free(sim_db);
        exit(1);
    }




for(i=0;i< NUM_OF_PAGES;i++){
	sim_db->page_table[i].valid=0;
	sim_db->page_table[i].dirty=0;
	sim_db->page_table[i].in_backing_store=0;
	sim_db->page_table[i].frame=0;

}
return sim_db;

}


//---------------------------------------------------------------------------------------------------------------------------------------------
//  method that read from logical address from the ram

char load ( sim_database * mem_sim , int address){
// cheack if the address  not ge out of  the addres range
if (cheack_if_valid_address(address)==-1 ){
 	perror("unvalid adrress ");
 	return '\0';
 }
int page= address/PAGE_SIZE;
int offset=address%PAGE_SIZE;
// cheack if the data finds in the memmory
if (mem_sim->page_table[page].valid!=0){
	int place = mem_sim->page_table[page].frame*PAGE_SIZE+offset;
	return mem_sim->main_memory[place];
}
// if the data not in the memmory-
// cheack if  we are in the part of the code or we in bss_heap_stack_size
	
	if (address>=mem_sim->text_length){// find in the varble zoone
		if (mem_sim->page_table[page].in_backing_store ==0){// cheack if the date not finds in the swap
			perror("tring to accsees to unallocated memory");
 			return '\0';
		}
	}
	//if the data finds in the swap we will cheacks if the memmory full
	if ( mem_sim->correntFrame>= NUM_OF_FRAME){
	
		moveToSwap(mem_sim);// cheack if corently frame that finds in the memmory  need  swap and do if need to
	}
		
	// write to the main memmory
	write_to_mainMemory(mem_sim, page, offset, NULL);
	int place = mem_sim->page_table[page].frame*PAGE_SIZE+offset;
	return mem_sim->main_memory[place];
}
//----------------------------------------------------------------------------------------------------------------------------------
// method that  write to logical address 
 
void store(sim_database * mem_sim , int address, char value){

if (address< mem_sim->text_length || address> SWAP_SIZE  ){
	perror(" invalid adrress");
 	return;
}
int i;
int page= address/PAGE_SIZE;
int offset=address%PAGE_SIZE;
char str[PAGE_SIZE];
if (mem_sim->page_table[page].valid!=0){// the data find in the memmory
	int place = mem_sim->page_table[page].frame*PAGE_SIZE+offset;
	mem_sim->main_memory[place] = value;
	mem_sim->page_table[page].dirty=1;
	return;
}
// if its not in the memmory 
if ( mem_sim->correntFrame>= NUM_OF_FRAME){//cheak memory full
	moveToSwap(mem_sim);// cheack if corently frame that finds in the memmory  need  swap and do if need to
}

if (mem_sim->page_table[page].in_backing_store ==0){// check if the adrres we get not   in the swap 
	
	for ( i=0;i<PAGE_SIZE;i++){// if not in the swap we insilize new  page in zero 
		str[i] = '0';
	}
	write_to_mainMemory(mem_sim, page, offset, str); //and write   page of zero to memmory 
}

else {
	write_to_mainMemory(mem_sim, page, offset, NULL);
}
int place = mem_sim->page_table[page].frame*PAGE_SIZE+offset;
mem_sim->main_memory[place] = value;// change  the nedd char
mem_sim->page_table[page].dirty=1;
}


//--------------------------------------------------------------------------------------------------------------------------------------------

//method that checks when there is no  more place in the memmory  and the address located in the varible zoon  if need to move the frame that corently in the memmory   that we want to replace ( by FIFO order)  to the swap 
void moveToSwap(sim_database * mem_sim){
	int pageToRemove = mem_sim->pageInFrame[mem_sim->correntFrame%6]; // cheak which page is in the frame 
	int startAddress= pageToRemove*PAGE_SIZE;// cheak if the page we want to replace  in the code zoon 
	if (startAddress>=mem_sim->text_length){// cheack on the page that we want delete
		if ( mem_sim->page_table[pageToRemove].dirty!=0){// it mean there were a changes and need to move the swap
			 char c[PAGE_SIZE];// array that save frame we want to delate from  the main memmory and move to the swap
			 int i;
			 int main_memory_index=(mem_sim->correntFrame%6)*PAGE_SIZE;
			 for(i=0; i<PAGE_SIZE;i++){
			 	c[i]=mem_sim->main_memory[main_memory_index+i];
			 }
			 // write to swap
			 lseek(mem_sim->swapfile_fd, startAddress, SEEK_SET);// for move the semmemn to the want place
			 int howMuch= write(mem_sim->swapfile_fd,c,PAGE_SIZE); 
			 if ( howMuch==-1){
				perror("couldnot write to the swap\n");
				return;
			}
			// to update  pagetable date
			 mem_sim->page_table[pageToRemove].dirty=0;
			 mem_sim->page_table[pageToRemove].in_backing_store = 1;
		}
	}
	mem_sim->page_table[pageToRemove].valid=0;
	mem_sim->page_table[pageToRemove].frame=0;
}


//------------------------------------------------------------------------------------------------------------------------------------------------

// method that write some page to the main memory
void write_to_mainMemory(sim_database * mem_sim, int page, int offset, char* str){
char c[PAGE_SIZE];
int startAddress= page*PAGE_SIZE;
int fd;
int i;
if(str == NULL){
	if (startAddress<mem_sim->text_length){
		fd = mem_sim->program_fd;
	}
	else {
		fd = mem_sim->swapfile_fd;
	}

	lseek(fd, startAddress, SEEK_SET);// for move the semmemn to the want place
	int howMuch= read(fd,c,PAGE_SIZE); 
	if ( howMuch==-1){
		perror("couldnot write to the swap\n");
		return;
	}
}
// str no null - for store option wen we have new page of zero  
else{
	for(i = 0; i<PAGE_SIZE;i++){
		c[i] = str[i];
	}
}
//write to the main memmory

for(i=0; i<PAGE_SIZE;i++){
	mem_sim->main_memory[(mem_sim->correntFrame%6)*PAGE_SIZE+i]=c[i];
}
// update data 
mem_sim->page_table[page].valid=1;
mem_sim->page_table[page].frame=mem_sim->correntFrame%6;
mem_sim->pageInFrame[mem_sim->correntFrame%6] = page;
mem_sim->correntFrame++;

}

//-------------------------------------------------------------------------------------------------------------------------------
int cheack_if_valid_address(int address){

if (address<0 || address> SWAP_SIZE){
	return -1;
}
return 1;
}

//--------------------------------------------------------------------------------------------------------------------------------------
//method that print  memmory's file data
void print_memory( sim_database * mem_sim){

int i=0;
for (i=0; i<MEMORY_SIZE;i++){
	printf("%d)\t[%c]\n",i,mem_sim->main_memory[i]);
}

}

//----------------------------------------------------------------------------------------------------------------------------
// method that print the swap file data 
void print_swap( sim_database * mem_sim){

lseek(mem_sim->swapfile_fd,0 , SEEK_SET);// for move the semmemn to the want place
char c;
int i;
printf("0)\t");
for(i = 0; i < SWAP_SIZE; i++)
{
	read(mem_sim->swapfile_fd,&c,1);
	if(i % 4 == 3)
		printf("[%c]\n%d)\t", c, i / 4 + 1);
	else
		printf("[%c]", c);
}
printf("\n\n");

}
//-----------------------------------------------------------------------------------------------------------------------------------
// method that clear the  all alloceted memmory
void clear_system(sim_database * mem_sim) {
	close(mem_sim->swapfile_fd);
	close(mem_sim->program_fd);
	free(mem_sim->page_table);
	free(mem_sim);
}
//-----------------------------------------------------------------------------------------------------------------------
