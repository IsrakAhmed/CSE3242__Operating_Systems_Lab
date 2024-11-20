#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/prctl.h>

int main(int argc, char *argv[]) {
	
    // Check if the number of arguments is 3
    if (argc != 4) {
		
        printf("Error: The number of arguments must be 3.\n");
		
        exit(1);
		
    }

    // Retrieve the arguments
    int num1 = atoi(argv[2]);
    int num2 = atoi(argv[3]);
	
    int result = 0;  // Variable to store result locally
	
	printf("result = %d\n",result);


    // Create the first child process for addition
    pid_t pid_addition = fork();
    
    if (pid_addition < 0) { // Fork failed
	
        printf("Fork failed for addition process.\n");
		
        exit(1);
		
    }
	
	else if (pid_addition == 0) { // Child process for addition
	
		// Set child process name to "addition"
		prctl(PR_SET_NAME, "addition", 0, 0, 0);
		
        result = num1 + num2;
        printf("ChildProcess-%d: addition_result = %d\n", getpid(), result);
		
		sleep(1);
		
        exit(0);
		
    }


    // Create the second child process for subtraction
    pid_t pid_subtraction = fork();

    if (pid_subtraction < 0) { // Fork failed
	
        printf("Fork failed for subtraction process.\n");
		
        exit(1);
		
    } 
	
	else if (pid_subtraction == 0) { // Child process for subtraction
	
		// Set child process name to "subtraction"
		prctl(PR_SET_NAME, "subtraction", 0, 0, 0);
	
        result = num1 - num2;
		
        printf("ChildProcess-%d: subtraction_result = %d\n", getpid(), result);
		
		sleep(1);
		
        exit(0);
		
    }


    // Wait for both child processes to finish
    int status;
    waitpid(pid_addition, &status, 0);
    printf("Child %d successfully finished.\n", pid_addition);
    
    waitpid(pid_subtraction, &status, 0);
    printf("Child %d successfully finished.\n", pid_subtraction);

    return 0;
}