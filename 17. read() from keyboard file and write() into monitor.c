/*

	Write a program to learn read() from keyboard file and write() into monitor file

*/


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


#define MAX_BUFFER_SIZE 1024  // Maximum size of buffer to read


int main() {
	
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;

    // Prompt the user to enter text
    write(1, "Enter some text: ", 17);

    // Read from the keyboard (standard input, file descriptor 0)
    bytesRead = read(0, buffer, MAX_BUFFER_SIZE);
	
    if (bytesRead < 0) {
		
        write(2, "Error reading input\n", 20);  // Write error to standard error
		
        return 1;
		
    }


    // Write the data read from keyboard to the monitor (standard output, file descriptor 1)
    bytesWritten = write(1, buffer, bytesRead);
	
    if (bytesWritten < 0) {
		
        write(2, "Error writing output\n", 21);  // Write error to standard error
		
        return 1;
		
    }


    return 0;
	
}