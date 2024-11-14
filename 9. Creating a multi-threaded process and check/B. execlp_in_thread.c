

/*


	B. If a thread invokes the exec() system call, does it replace the entire code of the process?

	Answer:	Yes. When the child process created by fork() invokes exec(), it replaces the entire code of the process,
		including all threads, with the new executable.


*/




#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void thread_controller(){
	execlp("./test", "test", NULL);
	printf("Hello from the thread\n");
	sleep(50);
}


int main(){
	pthread_t thread;
	
	pthread_create(&thread, NULL, (void*)thread_controller, NULL);
	
	printf("I am from the main: %d\n", getpid());
	
	pthread_join(thread, NULL);
	sleep(50);
}


