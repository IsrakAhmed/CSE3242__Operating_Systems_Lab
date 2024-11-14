

/*


	C. If exec() is called immediately after forking, will all threads be duplicated?

	Answer:	No. When exec() is called immediately after forking, all threads are replaced by the new process image,
		and no threads are duplicated in the new process.


*/



#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void thread_controller(){
	fork();
	execlp("./test", "test", NULL);
	printf("Hello from the thread\n");
	sleep(100);
}


int main(){
	pthread_t thread;
	
	pthread_create(&thread, NULL, (void*)thread_controller, NULL);
	
	printf("I am from the main: %d\n", getpid());
	
	pthread_join(thread, NULL);
	sleep(100);
}


