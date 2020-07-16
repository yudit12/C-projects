#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#define MAXNUM 510
#define BYE "bye\n" 

char* shell_format(); // shows the format shell
int count_spaces(char* str); // count num of space in the inpout
int split_by_qout(char** command, char* input); // split the input  by " "
int numOf_arguments(char** command, int rows); // count the realy num of args in the input
void final_command_arr(char** argv, char** arr, int rows); // the finall arr
void printArr(char** arr, int rows); //  print all args in arr

int main() {
    int cheacks;// cheack  the process statues
   
    char* input = (char*) malloc (sizeof(char) * MAXNUM);
    char* command_line = shell_format(); // print the shell
    char** command; // temp arr
    do {
        printf("%s",command_line);
        fgets(input, MAXNUM, stdin); // scan input from user
        char* temp = strdup(input); // creat copy of input
        temp = strtok(temp, " "); // get the first separate arg in the input
        if(strcmp(temp,BYE) == 0) { // if the input == bye\n
            break;
        }
        
        temp = strdup(input); // free at func count_spaces
        command = (char**) malloc (sizeof(char*) * count_spaces(temp) * 3); // 3 max number of needed memory tham may be need
        int rows = split_by_qout(command, input);
        int argc = numOf_arguments(command, rows);
        char** argv = (char**) malloc (sizeof(char*) * (argc + 1)); // +1 for NULL at the end of the array
        final_command_arr(argv, command, rows);
        pid_t id = fork(); 
        if(id < 0) { // if was error at the creat process
            printf("Failed to fork process\n");
            exit(1);
        }
            
        else if(id == 0) { // if its son
            int check_exe = execvp(argv[0], argv); 
            if(check_exe == -1) { // if the exec faild
                perror("exec has failed");
                exit(1);
            }
        }
            
        else {
            waitpid(id, &cheacks, 0); // wait for the son
            if(!WIFEXITED(cheacks)) { //if something wrong
                printf("child core dump\n");
                exit(1);
            }
        }
        
    } while(1);
    
    free(command_line);
    free(input);
    return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//method that checks and print the shell format
char* shell_format() { // set format
    char* host = (char*) malloc (sizeof(char) * 100);
    if(gethostname(host, 100) == -1) {// if wrong host
        host = "localhost"; 
    }
    
    char* login = getpwuid(getuid()) -> pw_name;
    if(login == NULL) { // if wrong login
        login = "login"; 
    }
    
    char* command_line = (char*) malloc (sizeof(char) * (strlen(login) + strlen(host) + 3)); // +3 chars => @,>,\0
    strcpy(command_line, login);
    strcat(command_line,"@");
    strcat(command_line, host);
    strcat(command_line, ">");
    free(host);
    free(login);
    return command_line;
}
//-----------------------------------------------------------------------------------------------------------------

//method that count the num of spaces in the inpout 
int count_spaces(char* str) { // count num of separated words
    int rows = 0;
    char* temp = strtok(str, " \n"); // cut until the first space or \n
    while(temp != NULL) { // while the string not end
        rows++; 
        temp = strtok(NULL, " \n"); // cut until the next space or \n
    }
    
    free(str); // str, not need any more
    return rows;
}
//-----------------------------------------------------------------------------------------------------------------
// method that split the input by " " and include it like one word and put it in the cpmmand arr when the first cell is the
// ram name
int split_by_qout(char** command, char* input) { // split the input to arr
    
    int rows = 0; //count num of args in command arr
    int qout = 0; // count  qout 
    if(input[0] == '\"') { // if the input start with "
        command[rows] = strdup("\""); // add " to the arr -> the next string is in "
        qout++;
    }
    
    char* str = strtok(input, "\"\n\0"); // cut input by ", \n and \0 
    while(str != NULL) { // while the input not end
        if(qout % 2 != 0) { // if qout % 2 != 0 -> this string was in "
            command[rows] = strdup("\""); // add " to the arr -> the next string is in "
            rows++;
        }
        
        qout++;
        command[rows] = strdup(str); // save the string in the command arr
        rows++;
        str = strtok(NULL, "\"\n\0"); // cut again
    }
    
    return rows; // return num of args in command arr
}
//--------------------------------------------------------------------------------------------------------------------
// method that count the num of the coomand arguments and return this count the realy num of args in the input
int numOf_arguments(char** command, int rows) { 
    int i;
    int num_of_args = 0;
    for(i = 0; i < rows; i++) {
        if(strcmp(command[i],"\"") != 0) { // if command[i] != '\"' -> count num of separate strings
            char* temp = strdup(command[i]); // temp free at func count_spaces
            num_of_args += count_spaces(temp); // get number of separate strings
        }
        
        else { 
            num_of_args++;
            i++; // i+2 -> 1 for " and 1 for the string in the "
        }
    }
    
    return num_of_args;
}
//-------------------------------------------------------------------------------------------------------------------------
//method that  create the final arr of the wanted command 
void final_command_arr(char** argv, char** arr, int rows) { // set the argv - the finall arr
    int i, j; // i "point" on arr, j "point" on argv
    for(i = 0, j = 0; i < rows; i++) { 
        if(strcmp(arr[i], "\"") != 0) { // 
            char* temp = strtok(arr[i], " "); // split by spaces
            while(temp != NULL) { 
                argv[j] = strdup(temp); // save in new cell
                j++; // go to the next cell in argv
                temp = strtok(NULL, " "); // cut again
            }
        }

        else { // if arr[i] == '\"' -> copy this string to argv
            i++; 
            argv[j] = strdup(arr[i]); // copy
            j++;
        }    
            
        free(arr[i]); // relese memory
    }
    
    free(arr); 
    argv[j] = NULL; 
}
//------------------------------------------------------------------------------------------------------------------
// print the arry for  myself cheack
void printArr(char** arr, int rows) { // test func, print all args in arr
    int i = 0;
    for(i = 0; i< rows; i++) {
        if(arr[i] != NULL) {
            printf("%s\n", arr[i]);
        }
    }
    
    if(arr[rows] != NULL) {
        printf("%s\n", arr[rows]);
    }
    
    else {
        printf("NULL\n");
    }
    
    printf("rows = %d\n", rows);
}
//-----------------------------------------------------------------------------------------------------------------------------
