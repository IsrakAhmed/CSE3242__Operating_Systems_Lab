/*


	Write a program to read from a named pipe and display it to the monitor using open(), read(), and write() system calls.


	
	1.	Run the Reader Program: In one terminal, run ./read_from_pipe.
	
	2.	Run the Writer Program: In another terminal, run your previous program to write to the named pipe.
		
	You should see the input from the writer displayed in the reader terminal.


*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main() {

    const char *pipePath = "/home/israk/my_named_pipe";

    char buffer[BUFFER_SIZE];
    int pipeFd;
    ssize_t bytesRead;

    // Open the named pipe for reading
    pipeFd = open(pipePath, O_RDONLY);

    if (pipeFd == -1) {

        perror("Failed to open named pipe");

        exit(EXIT_FAILURE);

    }


    printf("Reading from named pipe...\n");


    // Continuously read from the pipe and display the content
    while ((bytesRead = read(pipeFd, buffer, BUFFER_SIZE - 1)) > 0) {

        buffer[bytesRead] = '\0';  // Null-terminate the string

        // Write the content to the standard output
        if (write(STDOUT_FILENO, buffer, bytesRead) == -1) {

            perror("Failed to write to standard output");

            close(pipeFd);

            exit(EXIT_FAILURE);

        }

    }


    if (bytesRead == -1) {

        perror("Failed to read from named pipe");

    }


    // Close the named pipe
    close(pipeFd);

    return 0;

}
