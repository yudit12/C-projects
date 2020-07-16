#include "threadpool.h"

#include <stdio.h>
#include <stdlib.h>
//-----------------------------------------------------------------------------------------
//Private functions:
void error(char *msg) {  //print error msg and exit
	perror(msg);      
	exit(1); 
}
void destroy(threadpool* destroyme){//destroy the threadpool

	pthread_cond_destroy(&destroyme->q_not_empty);
	pthread_cond_destroy(&destroyme->q_empty);
	pthread_mutex_destroy(&destroyme->qlock);
	free(destroyme->threads);
	
	free(destroyme);

}


work_t * queue_dequeue(threadpool* threadpl){// dequeue the first element in the queue

	if(threadpl==NULL )
		 return NULL;
	if(threadpl->qsize==0)
		return NULL;
	work_t * work= threadpl->qhead;
	
	threadpl->qhead=threadpl->qhead->next;
	threadpl->qsize--;
	
	if(threadpl->qsize==0)
		threadpl->qtail=NULL;

	return work;
	
}
//-----------------------------------------------------------------------------------------
/**
 * create_threadpool creates a fixed-sized thread
 * pool.  If the function succeeds, it returns a (non-NULL)
 * "threadpool", else it returns NULL.
 */
 
threadpool* create_threadpool(int num_threads_in_pool){

	int i;

	if(num_threads_in_pool<1 || num_threads_in_pool> MAXT_IN_POOL){//1. input sanity check
		error("limit");

	}
	
	threadpool* threadpl = (threadpool *)malloc(sizeof(threadpool));
	if(threadpl ==NULL){

		 error("malloc");
	}	
	//2. initialize the threadpool structure:
	//and: 3. initialized mutex and conditional variables:
	
	threadpl->num_threads=num_threads_in_pool;	
	threadpl->qsize=0;	       				
	threadpl->threads=(pthread_t *)malloc(sizeof(pthread_t)*(threadpl->num_threads));	
	if(threadpl->threads ==NULL){

		 error("malloc");
	}
	
	threadpl->qhead=NULL;		
	threadpl->qtail=NULL;		
	if(pthread_mutex_init(&threadpl->qlock, NULL)!=0)//lock on the queue list 
		error("pthread_mutex_init");
	if(pthread_cond_init(&threadpl->q_not_empty, NULL)!=0) //non empty and empty condidtion vairiables
		error("q_not_empty");
	if(pthread_cond_init(&threadpl->q_empty, NULL)!=0)
		error("q_empty");
	threadpl->shutdown=0;          
	threadpl->dont_accept=0;     
	

	//4. create the threads
	for (i = 0; i < threadpl->num_threads; i++){

		if(pthread_create(&threadpl->threads[i], NULL, do_work, threadpl)!=0){ 

			 error("create");
		} 
			
	}

	return threadpl;
}


/**
 * dispatch enter a "job" of type work_t into the queue.
 * when an available thread takes a job from the queue, it will
 * call the function "dispatch_to_here" with argument "arg".
 *
 */
void dispatch(threadpool* from_me, dispatch_fn dispatch_to_here, void *arg){
	if(from_me==NULL){
		return; 
	}
	if (from_me->dont_accept==1){
		
		return; 
	}
	work_t* work = (work_t *)malloc(sizeof(work_t));
	if(work ==NULL){
    
		return;
	}
	work->routine= dispatch_to_here;
	work->arg= arg;
	work->next =NULL;
	//lock
	if(pthread_mutex_lock(&from_me->qlock)!=0){
		destroy(from_me);
		error("pthread_mutex_lock"); 
	} 
	//insert work to queue
	
	if(from_me->qsize==0){
		from_me->qhead = from_me->qtail = work;	

	}
	else{
		from_me->qtail->next=work;
		from_me->qtail=work;
	}
	//increase the num of work in the queue
	from_me->qsize++;
	

	//signal - wake up the threads that whait for empty queue
	if(pthread_cond_signal(&from_me->q_not_empty)!=0){
		destroy(from_me);
		error("pthread_cond_signal"); 
	}
	
	//unlock
	if(pthread_mutex_unlock(&from_me->qlock)!=0){
		destroy(from_me);
		error("pthread_mutex_unlock"); 
	}	

}


/**
 * The work function of the thread
 *
 */
void* do_work(void* p){
	if(p==NULL)
		return NULL;
	work_t * work;
	threadpool* threadpl=(threadpool*)p;
	while(1){
			
		//lock
		if(pthread_mutex_lock(&threadpl->qlock)!=0){
			destroy(threadpl);
			 error("pthread_mutex_lock"); 
		}
		//check flags
		
		if(threadpl->qsize==0 && threadpl->dont_accept==1) {
			if(pthread_cond_signal(&threadpl->q_empty)!=0){
				destroy(threadpl);
			 	error("pthread_cond_signal"); 
			}
			
			//return 0;
		}
		if(threadpl->shutdown==1){
			
			if(pthread_mutex_unlock(&threadpl->qlock)!=0){
				destroy(threadpl);
			 	error("pthread_mutex_unlock"); 
			}
			return NULL; 
		}
		//wait	
		
		if(threadpl->qsize==0){		
			if(pthread_cond_wait(&threadpl->q_not_empty, &threadpl->qlock)!=0){		
			
				destroy(threadpl);
			 	error("pthread_cond_wait"); 
			}
		}
		
		if (threadpl->shutdown==1){//check again destruction flag 
			
			if(pthread_mutex_unlock(&threadpl->qlock)!=0){
				destroy(threadpl);
			 	error("pthread_mutex_unlock"); 
			}
			return NULL;
		}
		
		
		work=threadpl->qhead;
		if(work!=NULL){
			work=queue_dequeue(threadpl);
			
			
			if(pthread_mutex_unlock(&threadpl->qlock)!=0)//unlock
			{
				destroy(threadpl);
			 	error("pthread_mutex_unlock"); 
			}
			work->routine(work->arg);

			
			free(work);
			work=NULL;
		}
		
		else if(pthread_mutex_unlock(&threadpl->qlock)!=0)//unlock
			{
				destroy(threadpl);
			 	error("pthread_mutex_unlock"); 
			}
	
		
		
		
	}
	
	
} 


/**
 * destroy_threadpool kills the threadpool, causing
 * all threads in it to commit suicide, and then
 * frees all the memory associated with the threadpool.
 */
void destroy_threadpool(threadpool* destroyme){
	
	if (destroyme == NULL)
		return;
		
	int i;
	destroyme->dont_accept=1;
	//LOCK
	if(pthread_mutex_lock(&destroyme->qlock)!=0){
		error("lock fails in destroy\n");
		return;
	}

	
	if(destroyme->qsize!=0){
		
		if(pthread_cond_wait(&destroyme->q_empty, &destroyme->qlock)!=0){
			destroy(destroyme);
			error("pthread_cond_wait");
		}
	}	
	
	destroyme->shutdown=1;

	
	//broadcast for q_notempty in order to wait for all threads
	if(pthread_cond_broadcast(&destroyme->q_not_empty)!=0){
		destroy(destroyme);
		error("pthread_cond_broadcast");
	}
	//unlock
	if(pthread_mutex_unlock(&destroyme->qlock)!=0){
		destroy(destroyme);
		error("pthread_mutex_unlock");
	}
	//join
	for( i = 0;i<destroyme->num_threads; i++) {
   		if(pthread_join(destroyme->threads[i], NULL)!=0){
   	     		destroy(destroyme);
   	    		error("pthread_join");
   		}
   	}
	//destroy (clean)
	destroy(destroyme);
	

	
	
}




