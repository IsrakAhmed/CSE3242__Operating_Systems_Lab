

/*

	A. If one thread in the process calls fork(), does the new process duplicate all threads or is the new process single-threaded?

	Answer:	In this case, only the calling thread (Thread 1) is duplicated in the new process after fork().
		The new process is single-threaded.

*/




#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


void thread_controller(){
	printf("Hello from the thread\n");
	
	fork();

	printf("HI from Process ID : %d and Parent ID : %d and Thread ID : %ld\n",getpid(),getppid(),pthread_self());
	
	//sleep(100);
}


int main(){
	pthread_t thread;
	
	pthread_create(&thread, NULL, (void*)thread_controller, NULL);
	
	printf("I am from the main\n");
	
	pthread_join(thread, NULL);
}


