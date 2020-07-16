#include "student.h"
#define N 30


int main(){

int numOfStudent=0;	 
 stud student[N];
 while(1){
	 char opretion[4];
	 char type;
	 char dir;
	scanf("%s", opretion);//  inpout  choice of the user
	// add new student- first enter id ,after enter name
	if (strcmp("ns" ,opretion)==0){
         addStudent (student,&numOfStudent); 
	}	
	//add new grade- first enter id student, after enter code grade and grade
    else if (strcmp("ng" ,opretion)==0){
		int i=findId(student,numOfStudent);	
		student[i].head_g=addGrade(student[i].head_g);
		
		
	}
	//add new income- first enter id student, after enter income date and income value
	else if (strcmp("ni",opretion)==0){
		int i=findId(student, numOfStudent);
		student[i].head_i=addIncome(student[i].head_i);
 
	}
	
	// sort student array
	else if(strcmp("sa",opretion)==0){
		scanf(" %c\n %c", &type,&dir);
			if('n'==type && 'a'==dir){// sort by name ascending order
				 sortStudentArray( student, numOfStudent,compByName_increaseOrder);
				 printf("hello");
			}
			else if('n'==type && 'd'==dir){// sort by name decrease order
				sortStudentArray(student, numOfStudent,compByName_decreaseOrder); 
			
			}
			else if('i'==type && 'a'==dir){// sort by id ascending order
				 sortStudentArray( student, numOfStudent,compStudentById_increaseOrder);
				 
			
			}
			else if('i'==type && 'd'==dir){// sort by id decrease order
				sortStudentArray( student, numOfStudent,compStudentById_decreaseOrder);
			
			}
			
	
	
	}
	//sort grade
	else if (strcmp("sg",opretion)==0){
	
		scanf(" %c\n %c", &type,&dir );
		if('c' == type && 'a' == dir){// sorting by code course in ascending order
			int sortBy[] = {1,1,1}; //{grade,code,incrase order}
			 sortAllStudent_LinkedList(student, numOfStudent,sortBy);

		}
		else if('c'==type && 'd'==dir){// sorting by code course in decrease order
			int sortBy[] = {1,1,-1}; //{grade,code,decrease order}
			 sortAllStudent_LinkedList(student, numOfStudent,sortBy);
		}
		
		else if('g'==type  && 'a'==dir){// sorting by course grade in ascending order
			int sortBy[] = {1,2,1}; //{grade,grade,increase order}
			 sortAllStudent_LinkedList(student, numOfStudent,sortBy);
		}
		
		else if('g'==type && 'd'==dir){ //// sorting by course grade in decrease order
			int sortBy[] = {1,2,-1};//{grade,grade,decrease order} 
			 sortAllStudent_LinkedList(student, numOfStudent,sortBy);
		}
		
		
	}
	
	// sort icome
	else if (strcmp("si",opretion)==0){
	
		scanf(" %c\n %c", &type,&dir );
		if('d'== type && 'a'== dir){// sorting by income datein ascending order
			int sortBy[] = {2,1,1}; //{income,date,increase order}
			 sortAllStudent_LinkedList(student, numOfStudent,sortBy);
		
		}
		else if('d'==type && 'd'==dir){// sorting by income date in decrease order
			int sortBy[] = {2,1,-1}; //{income,date,decrease order}
			 sortAllStudent_LinkedList(student, numOfStudent,sortBy);
		}
		
		else if('v'==type  && 'a'==dir){// sorting by income value in ascending order
			int sortBy[] = {2,2,1}; //{income,income value,increase order}
			 sortAllStudent_LinkedList(student, numOfStudent,sortBy);
			 
		}
		
		else if('v'==type && 'd'==dir){ //// sorting by income value in decrease order
			int sortBy[] = {2,2,-1};//{income,income value,decrease order} 
			 sortAllStudent_LinkedList(student, numOfStudent,sortBy);
		}
		
		
	}
	
	// print specific student information
	else if (strcmp("ws",opretion)==0){
		int i=findId(student, numOfStudent);
		printStudentInformation(student[i]);
		// printStudentgrade(student[i]);
		// printStudentIncome (student[i]);
		
	}
	else if(strcmp("pa",opretion)==0){
		printAllStudentsInformation (student, numOfStudent);
	}
	
	else if(strcmp("end" ,opretion)==0){
		int i;
		for(i = 0; i < numOfStudent; i++) {// realse memory
			grade *tempG;
			while(student[i].head_g != NULL){
				tempG = student[i].head_g;
				student[i].head_g = student[i].head_g->next;
				free(tempG);
			}
			inco *tempI;
			while(student[i].head_i != NULL){
				tempI = student[i].head_i;
				student[i].head_i = student[i].head_i->next;
				free(tempI);
			}
		}
	       break;
	}
		
	 
 }
	return 0;
}