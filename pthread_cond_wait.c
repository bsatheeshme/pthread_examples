#include <stdio.h> 
#include <sys/syscall.h>
#include <pthread.h> 
int done = 1; 

// Declaration of thread condition variable 
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER; 

// declaring mutex 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

void* calls(void* ptr) 
{ 
	// using pthread_self() get current thread id 
	printf("In function \nthread id = %ld\n", syscall(SYS_gettid)); 
	pthread_mutex_lock(&lock); 
	if (done == 1) { 

		// let's wait on conition variable cond1 
		done = 2; 
		printf("Waiting on condition variable cond1 dataowner[%d]\n",lock.__data.__owner); 
		pthread_cond_wait(&cond1, &lock); 
		printf("Waiting on condition variable cond2 dataowner[%d]\n",lock.__data.__owner); 
	} 
	else { 

		// Let's signal condition variable cond1 
		printf("Signaling condition variable cond1 dataowner[%d]\n",lock.__data.__owner); 
		pthread_cond_signal(&cond1); 
	} 

	// release lock 
	pthread_mutex_unlock(&lock); 

	printf("Returning thread\n"); 

	return NULL; 
} 

int main() 
{ 
	pthread_t thread[2]; // declare thread 
	for(int i=0;i<2;i++)
		pthread_create(&thread[i], NULL, calls, NULL); 
	printf("In main \nthread id = %ld\n", thread[0]);  
	printf("In main \nthread id = %ld\n", thread[1]);  
	pthread_join(thread[0], NULL);  
	pthread_join(thread[1], NULL);  
	return 0; 
} 
