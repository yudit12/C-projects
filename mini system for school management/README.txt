

In this exercise, we need to create an array of students that allow the user save his needed data.
Each cell in the array represents specifies student.
and each student has the following features: name,id and pointer to a linked list  that contain information of the students grade/ incomes
The program should allow user the following:
1) add new student
2) adding new grade for each student
3) adding new income for each student
4) print student information
4) print  all student's infromation
5)able sort the array as follows: name or id
6)able sort each student incomes/ grades as follows: 
  for student's incomes- income date or income value
  for student's grades- course grade or the grade value
 *Please note that each kind of sorting allows you to sort in ascending or descending order
 
 
 
how the user  should to compile the program?.
we create The file that unites all the files together called makefile
this file concerned to compile all the files together.
after saving each file when we write in the terminal "make"  all file pass compiling.

for start runing the progrm:
you should write in the terminal ./prog1.
after , start entering the wanted operation.
if you wonna rum  my tester to the program - write in the terminal ./prog1<tester1.txt

in this program we have few file:
The files are divided to files with extension ".h" and files with extension ".c"

in files with extension ".h" we :import the needed liberys to use of method
								 saved wanted structures
								 declarations to methods that implementation  in the file with extension ".c" with the same name.
								 
in files with extension ".c" we	: import the file. h with the same name and implementate all  method that relate to the file

in this programe  the files are: student.c/.o, grade.c/.o ,income.c/.o and main.c/
 *in main.c  we asked the wanted operation  and scan it for use of the need function.
 
needed inpot :
 *in each function enter the inpout in the writing order (wirhout the "," or " ")
1)add new student: please enter "ns" after wanted id  and name
2) adding new grade for an student: please enter  "ng", wanted id, grade code and grade value
3) adding new income for an student: please enter "ni" , wanted id, income dateand income value
4) print student information: enter "ws" and wanted student id;
5) print all students information: enter "pa"
6) for sort the student array-  by name in increase order please enter: n, a 
						        by  in decrease order please enter: n, d 
						        by id in increase order please enter: i,a
						        by id in increase order please enter: i,d
								
7)for sort all incomes- plese enter "si"
                       by income date in increase order please enter: d,a
                       by income date in decrease order please enter: d,d
					   by income value in increase order please enter: v,a 
					   by income value in decrease order please enter: v,d
					   
8) for sort all grades- please enter "sg"
                       by grade code in increase order please enter: c,a
					   by grade code in decrease order plese enter : c,d
					   by value of the grade in increase order please enter: g,a
					   by value of the grade in decrease order please enter: g,d
					   
9) for exsit  plese enter: "end".



 The resulting output:

1) for entering "ws" : id, student name
                      grade code  vale of the grade , ...... 
					  income date  income value, .............
					  
				for example: 123, Avia
			                 123456 100, 876543 95, 456765 93
					         10/12/16 50000, 08/12/16 1000000
		            
                     		
2) for entering "pa" : In similar to above  but for each student.
3)for sorting student array ,"sa"  : sortted array by name or id
4) for entring "sg" : all grades will be sorted by the  user's choice.
5) for entering "si": all incomeד will be sorted by the user's choice. 


explention to my tester:

I enter student with two grades end two incomes  and print the array. 
then I sort the array by income date ("si") in increasing order and print the array again ("pa").
 then I add new student ("ns") and add him three  grades and print is information ("ws").
 then I sort again by income but by income value in decrease order (changed all entered incomes)
 and print all student again.
 then I add another new student  and add him two income and two grades. and sort all by grade("ng") by grade code in increase order .
 again sort by income by income date in incrase order.
 and finally sort the student array by id in decrease order
 ,print all student " pa" 
 and enter "end"
 

 

 					  
					   
					


							 


  
  
  
