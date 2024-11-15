/*


	The Fibonacci sequence is the series of numbers: 0, 1, 1, 2, 3, 5, 8, ...

	Formally, it can be expressed as:

	fib(0) = 0
	fib(1) = 1
	fib(n) = fib(n−1) + fib(n−2)

	Write a multithreaded program that generates the Fibonacci sequence. This program should work as follows:
	On the command line, the user will enter the number of Fibonacci numbers that the program is to generate.
	The program will then create a separate thread that will generate the Fibonacci numbers, placing the sequence
	in shared data (an array is probably the most convenient data structure). When the thread finishes execution,
	the parent thread will output the sequence generated by the child thread. The parent thread will wait for the
	child thread to finish before outputting the Fibonacci sequence.



*/


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int *fibonacci_series;
int count;

void fibonacci_generator(){
	fibonacci_series = (int*) malloc(count*sizeof(int));
	
	fibonacci_series[0] = 0;
	fibonacci_series[1] = 1;
	
	for(int i=2;i<count;i++){
		fibonacci_series[i] = fibonacci_series[i-1] + fibonacci_series[i-2];
	}	
}


int main(int argc, char* argv[]){

	if(argc !=2){
		printf("Please provide the febonacci number.");
		exit(EXIT_FAILURE);
	}

	count = atoi(argv[1]);
	
	
	pthread_t thread;
	
	pthread_create(&thread, NULL, (void*)fibonacci_generator, NULL);
	pthread_join(thread, NULL);
	
	for (int i=0;i<count;i++){
		printf("%d ", fibonacci_series[i]);
	} 
	
	printf("\n");

	
}
