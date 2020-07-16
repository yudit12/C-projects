# include "grade.h"

//---------------------------------------------------------------------------------------------------------------------------------------
 // method that return pointer of type grade
 // for adding new grade plese enter course code after enter the grade  
 grade* addGrade ( grade *head_g ){

	//(grade* )represent pointer of type grade
	grade *newGrade= (grade*)malloc(sizeof(grade));
	int tempCode;
	int tempGrad;
	
	scanf("%d",&tempCode);//add course code
	scanf("%d",&tempGrad);// add course grade
	newGrade->code=tempCode;
	newGrade->grad=tempGrad;
	newGrade->next=head_g;
	
	return newGrade;
	

}
//-----------------------------------------------------------------------------------------------------------------------------------------
// print one grade
void printGrade(void *node){
	grade *temp=(grade*)node;// douncusting to pointer type grade
	printf("%d %d",temp->code, temp->grad);
	
}

//----------------------------------------------------------------------------------------------------------------------------------------
void* nextGrade(void * node){// the method return pointer to the nextGrade type void
	grade *temp= (grade*)node;
	temp=temp->next;
	return (void*)temp;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// method that compare grade by his curse code in increaseOrder 
int compareGradeByCode__increaseOrder(void *item1, void *item2){
	grade *temp1= (grade*)item1;
	grade *temp2= (grade*)item2;
	return temp1->code-temp2->code; /// return int>0 if it increaseOrder;
	
}
//--------------------------------------------------------------------------------------------------------------------------------------------
// method that compare grade by his curse code in decreaseOrder
int compareGradeByCode__decreaseOrder(void *item1, void *item2){
	grade *temp1= (grade*)item1;
	grade *temp2= (grade*)item2;
	return (temp1->code-temp2->code)*(-1); /// return int>0 if it decreaseOrder;
}

//-------------------------------------------------------------------------------------------------------------------------------------------
// method that compare grade by his value in increaseOrder
int compareGradeByGrade__increaseOrder(void *item1, void *item2){
	grade *temp1= (grade*)item1;
	grade *temp2= (grade*)item2;
	return temp1->grad-temp2->grad;
	
	
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
// method that compare grade by his value in decreaseOrder
int compareGradeByGrade__decreaseOrder(void *item1, void *item2){
	grade *temp1= (grade*)item1;
	grade *temp2= (grade*)item2;
	return (temp1->grad-temp2->grad)*(-1);
	
}
//-------------------------------------------------------------------------------------------------------------------------------------------
// swap betwwen two grades in the linke list
void swapGrade (void *item1, void *item2){
	grade *temp1= (grade*)item1;
	grade *temp2= (grade*)item2;
	int tempCode;
	int tempGrade;
	tempCode=temp1->code;
	tempGrade=temp1->grad;
	temp1->code=temp2->code;
	temp1->grad=temp2->grad;
	temp2->code=tempCode;
	temp2->grad=tempGrade;
	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------




	