
#include "income.h"

//------------------------------------------------------------------------------------------------------
//method add student income
//first please enter the date of the income after please enter income value
inco* addIncome ( inco *head_i){
	inco *newIncome= (inco*)malloc(sizeof(inco));
	char tempDate[MAX_STR_LEN];
	int tempIncome;
	scanf("%s", tempDate);
	scanf("%d", &tempIncome);
	strcpy(newIncome->date,tempDate);
	newIncome->income=tempIncome;
	newIncome->next=head_i;
	return newIncome;
	
}

//------------------------------------------------------------------------------------------------------------------------
//method  print the current income
void printIncome(void *node){
	inco *temp=(inco*)node;// dounCusting to pointer type grade
	printf("%s %d",temp->date, temp->income);
	
}

//-------------------------------------------------------------------------------------------------------------------------
// method that return pointer to the next income
void* nextIncome(void * node){// the method return pointer type void
	inco *temp= (inco*)node;
	temp=temp->next;
	return (void*)temp;
}

//----------------------------------------------------------------------------------------------------------------------------
// method that compare incame by date increaseOrder
int compareIncomeDate_increaseOrder(void *item1, void *item2){
	inco *temp1= (inco*)item1;
	inco *temp2= (inco*)item2;
	
	return strcmp(temp1->date, temp2->date);// return int>0 if it increaseOrder;
}

//---------------------------------------------------------------------------------------------------------------------
//method that  compare income date in decreaseOrder 
int compareIncomeDate_decreaseOrder(void *item1, void *item2){
	inco *temp1= (inco*)item1;
	inco *temp2= (inco*)item2;
	return (strcmp(temp1->date, temp2->date))*(-1);// return int>0 if it decreaseOrder;
	
}
//------------------------------------------------------------------------------------------------------------------------------------
// method that compare income by  value in increaseOrder
int compareIncomeDposite_increaseOrder(void *item1, void *item2){
	inco *temp1= (inco*)item1;
	inco *temp2= (inco*)item2;
	return temp1->income-temp2->income;
}

//-----------------------------------------------------------------------------------------------------------------------------------
 // method that compare income by  value in increaseOrder
 int compareIncomeDposite_decreaseOrder(void *item1, void *item2){
	 inco *temp1= (inco*)item1;
	inco *temp2= (inco*)item2;
	return (temp1->income-temp2->income)*(-1);
	
}
//--------------------------------------------------------------------------------------------------------------------------------------

// swap between to incomes in the link list
void swapIncome (void *item1, void *item2){
	inco *temp1= (inco*)item1;
	inco *temp2= (inco*)item2;
	char tempDate[MAX_STR_LEN];
	int tempDeposite;
	strcpy(tempDate,temp1->date);
	tempDeposite=temp1->income;
	strcpy(temp1->date,temp2->date);
	temp1->income=temp2->income;
	strcpy(temp2->date,tempDate);
	temp2->income=tempDeposite;
	
	
}


//---------------------------------------------------------------------------------------------------------------------------------------



