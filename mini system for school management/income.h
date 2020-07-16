#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAX_STR_LEN 20 

//The file contains the attributes of the income and declarations to incomes methods  
 typedef struct incomes{
	int income;// num of income 
	char date[MAX_STR_LEN];// date of the day starting income
	struct incomes *next;
} inco;

inco* addIncome ( inco *head_i);
void printIncome(void *node);
void* nextIncome(void *node);
int compareIncomeDate_increaseOrder(void *item1, void *item2);
int compareIncomeDate_decreaseOrder(void *item1, void *item2);
int compareIncomeDposite_increaseOrder(void *item1, void *item2);
int compareIncomeDposite_decreaseOrder(void *item1, void *item2);
void swapIncome (void *item1, void *item2);