#include "student.h"


//----------------------------------------------------------------------------------------------------------------------
// the method add new student
// first user need to enter wanted id and after enter the wanted name
 void addStudent( stud student[], int *numOfStudent ){
	 
	 scanf ("%s\n%s", student[*numOfStudent].id,student[*numOfStudent].name);
	 student[*numOfStudent].head_g=NULL;
	 student[*numOfStudent].head_i=NULL;
	
	(*numOfStudent)++;
	
 }
 //-------------------------------------------------------------------------------------------------------------------------------------
 // print all array
void printAllStudentsInformation (stud student[],int numOfStudent){
	int i;
	for(i=0;i<numOfStudent;i++){
		 printStudentInformation(student[i]);
		
	}
}	
	

//---------------------------------------------------------------------------------------------------------------
// print on student
 void printStudentInformation (stud student){

	printf("%s, %s\n",student.id, student.name);
	print_list(student.head_g, printGrade, nextGrade);
	print_list(student.head_i, printIncome, nextIncome);
	
}


//----------------------------------------------------------------------------------------------------------------------------------------------
 /* method I didnt use, writing diffrent from recorded    */
void printStudentgrade (stud student){
 
printf("%s, %s\n",student.id, student.name);

	grade *tempG= student.head_g;
		while(tempG!=NULL){
			printf("%d %d,",tempG->code, tempG->grad);
			tempG=tempG->next;
			if(tempG->next==NULL){
				printf("%d %d",tempG->code, tempG->grad);
				tempG=tempG->next;
	
			}
			
		}
		printf("\n");




}


//-------------------------------------------------------------------------------------------------------------------------------------------------
/* method I didnt use, writing diffrent from record  */
void printStudentIncome (stud student){
		
	inco *tempI=student.head_i;
		while(tempI!=NULL){
			printf("%s %d,",tempI-> date,tempI-> income);
			tempI=tempI->next;
			if(tempI->next==NULL){
				printf("%s %d",tempI-> date, tempI-> income);
				tempI=tempI->next;
			}
			
		}
		printf("\n");
}





//--------------------------------------------------------------------------------------------------------------

// method that find the wanted student id by cheking if there is id in the array that equal to the inpot
//  
int findId(stud student[],int numOfStudent){
		char id[MAX_STR_LEN];
		scanf("%s",id);
        int i;
		for (i=0; i<numOfStudent;i++){
			if(strcmp(id ,student[i].id)==0){
				break;
			
			}
		}
		
		return i;	
}
//-----------------------------------------------------------------------------------------------------------------

// compare student by id increaseOrder(
int compStudentById_increaseOrder(const stud *s1, const stud *s2){
	return strcmp(s1->id,s2->id);// the method return  int >0 if s1>s2;

}


//----------------------------------------------------------------------------------------------------------------------

// compare student by id decreaseOrde(
int compStudentById_decreaseOrder(const stud *s1, const stud *s2){
	return strcmp(s1->id,s2->id)*(-1);// the method return  int>0 if s1.id>s2.id;

}

//--------------------------------------------------------------------------------------------------------------------
// compare student by name in increaseOrder 
int compByName_increaseOrder(const stud *s1, const stud *s2){
	return strcmp(s1->name,s2->name);//the method return  int>0 if s1.name>s2.name
}

//-------------------------------------------------------------------------------------------------------------------------
//  compare student by name in decreaseOrder
int compByName_decreaseOrder(const stud *s1, const stud *s2){
	return strcmp(s1->name,s2->name)*(-1);//
	
}

//---------------------------------------------------------------------------------
// method that swap bettwen to student in the array
void  swap( stud *s1,  stud *s2){
	
	stud *temp=(stud*)malloc(sizeof(stud));
	*temp=*s1;
	*s1=*s2;
	*s2=*temp;
	free(temp);
}

//-------------------------------------------------------------------------------------------------
// method that sort the student array   by pointer to each of the foe method mention above for the order of the sort
	void sortStudentArray(stud student[],int numOfStudent, int (*cmp)(const struct stud *s1, const struct stud *s2)){
		int i,j;
		for (i=0;i<numOfStudent; i++){
			for(j=i+1; j<numOfStudent;j++){
				if(cmp(&student[i],&student[j])>0){
					swap(&student[i],&student[j]);
				}
			}
		}
	}
	
	
//-----------------------------------------------------------------------------------------------------------------
// ganeri method that print the wanted list(grade/income) by get: pointer to the head of the wanted list , pointer to the method that return pointer
// to the next  wanted item and pointer to the method  that print the wanted item. and  

void print_list( void *list, void (*print_item)(void *item), void* (*get_next)(void *item)){
	while(list!=NULL){
		print_item(list);
		if(get_next(list) != NULL){
			printf(", ");
		}
		list=get_next(list);
	}
	printf("\n");


}   

//------------------------------------------------------------------------------------------------------------------------
// mtehod that sort  wanted link list (grade/ income) by geting: pointer to the pointer of the wanted list
// , pointer to method that return in  which order to sort end by which feature (grade: grade/code, income: date/ income value)
//,  pointer to method swap of the wanted list( the swap opend when the before mention method return value >0 )
//, pointer to the next wanted item
  
void sort_LinkedList(void **list , int (*cmp)(void *item1, void *item2)
,void (*swap) (void *item1, void *item2),void *(*get_next)(void *item),void (*insertNode)(void **list, void* node)){
	void *p,*q;
	
	for(p=*list;p!=NULL;p = get_next(p)){
	
		for(q=*list;get_next(q)!=NULL;q=get_next(q)){
		if(cmp(q,get_next(q))>0)// swap if 

			swap(q,get_next(q));
		}
	}

	
}

//-----------------------------------------------------------------------------------------------------------------------
// method that sort all list .
// the method get array typeOfSort lenght = 3 each cell define other wanted sort opertion
// first cell : select between which linkList  grade (sign by 1) and income  (sign by 2)
// second cell: select the element to sort by him  
// third cell: select in which order to sort  (incrase/ decrease)
void sortAllStudent_LinkedList(stud student[], int numOfStudent,int typeOfSort[]){
	void ** list;
	void (*swap) (void *item1, void *item2);
	void *(*get_next)(void *item);
	int (*cmp)(void *item1, void *item2);
	int i = 0;
	
	for(i=0;i<numOfStudent;i++){
	
	if(typeOfSort[0] == 1){// 1= grade
		
		list=(void**)(&student[i].head_g);
		swap = swapGrade;
		get_next=nextGrade;
		if(typeOfSort[1] == 1 && typeOfSort[2]==1){// typeOfSort[1]=1 code, typeOfSort[2]==1 increseorder.
			cmp =  compareGradeByCode__increaseOrder;
		}
		else if(typeOfSort[1] == 1 && typeOfSort[2]== -1){// typeOfSort[1]=1 code, typeOfSort[2]==-1 decrease order.
			cmp=compareGradeByCode__decreaseOrder;
		}
		else if(typeOfSort[1] == 2 && typeOfSort[2]==1){// typeOfSort[1]=2 grade, typeOfSort[2]==1 increseorder.
			cmp =  compareGradeByGrade__increaseOrder;
		}
		else if(typeOfSort[1] == 2 && typeOfSort[2]== -1){// typeOfSort[1]=2 grade, typeOfSort[2]==-1 decrease order.
			cmp=compareGradeByGrade__decreaseOrder;
		}
		
		
	}
	
	else if(typeOfSort[0] == 2) {//income
		list=(void**)(&student[i].head_i);	
		swap= swapIncome;
		get_next=nextIncome;
		if(typeOfSort[1] == 1 && typeOfSort[2]==1){// typeOfSort[1]=1 date, typeOfSort[2]==1 increseorder.
			cmp =  *compareIncomeDate_increaseOrder;
		}
		else if(typeOfSort[1] == 1 && typeOfSort[2]== -1){// typeOfSort[1]=1 date, typeOfSort[2]==-1 decrease order.
			cmp=*compareIncomeDate_decreaseOrder;
		}
		else if(typeOfSort[1] == 2 && typeOfSort[2]==1){// typeOfSort[1]=2 income, typeOfSort[2]==1 increseorder.
			cmp =  *compareIncomeDposite_increaseOrder;
		}
		else if(typeOfSort[1] == 2 && typeOfSort[2]== -1){// typeOfSort[1]=2 income, typeOfSort[2]==-1 decrease order.
			cmp=*compareIncomeDposite_decreaseOrder;
		}
		
	}

	sort_LinkedList( list,cmp ,swap,get_next,NULL);
	
	}
	
}

	