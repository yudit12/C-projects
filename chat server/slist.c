#include "slist.h"
#include <stdlib.h>
#include <stdio.h>

#define FAILURE -1
#define SUCCESS 0
//---------------------------------------------------------
//pravte methods:
slist_node_t* create_node(void* data);
//---------------------------------------------------------------------------------
void slist_init(slist_t * list){
	if(list==NULL){
		return;
	}
	slist_head(list)= NULL;
	slist_tail(list)= NULL;
	slist_size(list)= 0;


}
/** Pop element by sd from the list**/
void pop_by_sd(slist_t * l,int sd){


	if(l==NULL || slist_head(l)==NULL)
		return;
	slist_node_t * nodeTemp= slist_head(l);
	slist_node_t * curNode= slist_head(l);

	if(*((int*)slist_data(nodeTemp))==sd){//if need to free the first node

		free(slist_pop_first(l));
		return;
	}

	while(slist_next(nodeTemp)!=NULL ){
		if( *((int*)slist_data(slist_next(nodeTemp)))==sd){

			if(slist_next(slist_next(nodeTemp))==NULL)
				slist_tail(l)=nodeTemp;
			curNode=slist_next(nodeTemp);
			slist_next(nodeTemp)=slist_next(slist_next(nodeTemp));
			slist_size(l)--;
			free(curNode->data);
			free(curNode);


			return;

		}
		nodeTemp=slist_next(nodeTemp);
	}
}

//-------------------------------------------------------------------
//private method to remove node from the list
void remove_node(slist_t* list ,void* fd){

	if (list== NULL){
		return ;
	}

	if(slist_head(list)== NULL){//empty list

		return;
	}
	int data=*((int*)slist_data(slist_head(list)));

	int fd1=*((int*)fd);

	if(data == fd1 && slist_size(list)==1){
		slist_node_t* temp = slist_head(list);
		slist_head(list)=NULL;
		slist_tail(list)=NULL;
		slist_size(list)=0;
		free(slist_data(temp));
		free(temp);
		return;
	}

	slist_node_t * current=slist_head(list);
	slist_node_t *previous=current;
	while(current != NULL)
	{

		if(*((int*)slist_data(current))== fd1){

			slist_node_t *ptr=current;
			slist_next(previous)=slist_next(current);
			if (current == slist_head(list)){
				slist_head(list) = slist_next(slist_head(list));

			}
			else if (current == slist_tail(list)){
				slist_tail(list) =previous;
			}

			free(slist_data(ptr));
			free(ptr);
			slist_size(list)-=1;
			return;
		}
		previous=current;
		current=slist_next(current);
	}
	slist_size(list)-=1;
	return;
}

//-----------------------------------------------




//----------------------------------------------

//-----------------------------------------------------
// remove first ekment and return its data
void* slist_pop_first(slist_t* list){
	if (list== NULL){
		return NULL;
	}
	if(slist_head(list)== NULL){//empty list
		return NULL;
	}
	void *tempData=slist_data(slist_head(list));// save value
	if(slist_size(list)==1){

		free(slist_head(list));
		slist_head(list)=NULL;
		slist_tail(list)=NULL;
		slist_size(list)=0;
		return tempData;
	}

	slist_node_t* temp=slist_head(list);
	slist_head(list)=slist_next(slist_head(list));// move head
	free(temp);
	temp =NULL;
	slist_size(list)-=1;
	return tempData;

}
//----------------------------------------------------------------------------------------
//add as last node of the list
int slist_append(slist_t* list,void* data){
	if (list== NULL || data==NULL){
		return FAILURE;
	}
	slist_node_t *newNode= create_node(data);
	if(newNode== NULL){
		return  FAILURE;
	}
	if(slist_head(list)== NULL){// empty
		slist_head(list)=newNode;
		slist_tail(list)=newNode;
	}
	else {
		slist_next(slist_tail(list))=newNode;
		slist_tail(list)=newNode;

	}
	slist_size(list)+=1;
	return SUCCESS;
}
//----------------------------------------------------------------------------------------------------------
//add as first node of the list
int slist_prepend(slist_t* list,void* data){
	if (list==NULL || data==NULL){
		return  FAILURE;
	}
	slist_node_t *newNode= create_node(data);
	if(newNode== NULL){
		return  FAILURE;
	}
	if(slist_head(list)==NULL){
		slist_head(list)=newNode;
		slist_tail(list)=newNode;
	}
	else{
		slist_next(newNode)=slist_head(list);
		slist_head(list)=newNode;
	}
	slist_size(list)+=1;
	return SUCCESS;



}

//------------------------------------------------------------------------------------------
// method that append list 2 to list1 by duplicate each node of list2 and and enter the value to the new node by creating
// a pointer to the data in the old list2
int slist_append_list(slist_t* to, slist_t* from){


	if(to==NULL || from==NULL){//
		return  FAILURE;

	}
	slist_node_t* temp=slist_head(from);

	while(temp!=NULL){
		int check=slist_append(to,slist_data(temp));

		if(check==SUCCESS){
			temp=slist_next(temp);// temp++
		}
		else{// check =-1
			return  FAILURE;
		}
	}
	return SUCCESS;

}

//---------------------------------------------------------------------------------------------------------------
// pravite method  - create new node

slist_node_t* create_node(void* data){

	slist_node_t *newNode= malloc(sizeof( slist_node_t));
	if(newNode==NULL){
		return NULL;
	}
	slist_data(newNode)=data;
	slist_next(newNode)=NULL;

	return newNode;
}
//----------------------------------------------------------------------------------------------------------

// destroy of the list
void slist_destroy(slist_t* list,slist_destroy_t flag){
	if (list==NULL){
		return;
	}


	while(slist_head(list)!=NULL){
		slist_node_t* temp=slist_head(list);
		if(flag==SLIST_FREE_DATA){
			free(slist_data(temp));
			
		}

		slist_head(list)=slist_next(temp);
		free(temp);
		temp=NULL;

	}

	// free list  one the main



}
//--------------------------------------------------------------------------


