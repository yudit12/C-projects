#include "grade.h"
#include "income.h"


//The file contains the attributes of the student and declarations to students methods  
 typedef struct stud {
	 char id [MAX_STR_LEN];
	 char name [MAX_STR_LEN];
	 grade *head_g ;
	 inco *head_i ;
 }stud;
 
void addStudent( stud student[], int *numOfStudent);
void printStudentInformation(stud student);
void printAllStudentsInformation (stud student[],int numOfStudent);
int findId(stud student[],int numOfStudent);
void sortStudentArray(stud student[],int numOfStudent, int (*cmp)(const struct stud *s1, const struct stud *s2));
//void printStudentIncome (stud student);
//void printStudentgrade (stud student);
int compStudentById_increaseOrder(const stud *s1, const stud *s2);
int compStudentById_decreaseOrder(const stud *s1, const stud *s2);
int compByName_increaseOrder(const stud *s1, const stud *s2);
int compByName_decreaseOrder(const stud *s1, const stud *s2);
void  swap( stud *s1,  stud *s2);
void sortStudentArray(stud student[],int numOfStudent, int (*cmp)(const struct stud *s1, const struct stud *s2));
void print_list( void *list, void (*print_item)(void *item), void *(*get_next)(void * item));
void sort_LinkedList(void **list , int (*cmp)(void *item1, void *item2)
,void (*swap) (void *item1, void *item2),void *(*get_next)(void *item),void (*insertNode)(void **list, void* node));
void sortAllStudent_LinkedList(stud student[], int numOfStudent,int typeOfSort[]);

