
#include<stdio.h>
#include<stdlib.h>

//The file contains the attributes of the grade and declarations to grades methods  
typedef struct grades{

	int code;
	int grad;
	struct grades *next; 
}grade;

grade* addGrade ( grade *head_g );
void printGrade(void *node);
void* nextGrade(void * node);
int compareGradeByCode__increaseOrder(void *item1, void *item2);
int compareGradeByCode__decreaseOrder(void *item1, void *item2);
int compareGradeByGrade__increaseOrder(void *item1, void *item2);
int compareGradeByGrade__decreaseOrder(void *item1, void *item2);
void swapGrade (void *item1, void *item2);