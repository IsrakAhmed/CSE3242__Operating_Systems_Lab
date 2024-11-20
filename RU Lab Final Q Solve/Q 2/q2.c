#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int global_variable = 0;  // Global variable to demonstrate data inconsistency

pthread_mutex_t mutex;    // Mutex to protect the global variable


void *increment_without_mutex(void *arg) {
	
    for (int i = 0; i < 100000; i++) {
		
        global_variable++;  // Increment the global variable without mutex
		
		/*int temp = global_variable;
		temp = temp + 1;
		global_variable = temp;*/
		
    }
	
    return NULL;
	
}


void *increment_with_mutex(void *arg) {
	
    for (int i = 0; i < 100000; i++) {
		
        pthread_mutex_lock(&mutex);  // Lock the mutex
		
		/*int temp = global_variable;
		temp = temp + 1;
		global_variable = temp;*/
		
        global_variable++;  // Increment the global variable safely
		
        pthread_mutex_unlock(&mutex);  // Unlock the mutex
    }
	
    return NULL;
}


int main() {
	
    pthread_t t1, t2;

    // Part 1: Demonstrating data inconsistency
    global_variable = 0;
	
    printf("Demonstrating data inconsistency...\n");

    // Create two threads incrementing without a mutex
    pthread_create(&t1, NULL, increment_without_mutex, NULL);
    pthread_create(&t2, NULL, increment_without_mutex, NULL);

    // Wait for both threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // The expected result should be 200000, but due to race conditions, it will be lower.
    printf("Final value without mutex: %d\n", global_variable);


    // Part 2: Solving data inconsistency with mutex
    printf("Solving data inconsistency using mutex...\n");
	
    global_variable = 0;  // Reset the global variable

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create two threads incrementing with a mutex
    pthread_create(&t1, NULL, increment_with_mutex, NULL);
    pthread_create(&t2, NULL, increment_with_mutex, NULL);

    // Wait for both threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // The result will be 200000 as expected
    printf("Final value with mutex: %d\n", global_variable);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
	
}