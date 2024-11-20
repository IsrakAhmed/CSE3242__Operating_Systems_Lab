/*


	Write a program to read from a normal file and display to the monitor using open(), read(), and write() system calls.


*/


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


#define BUFFER_SIZE 1024  // Define the buffer size


int main() {

    int fd;                // File descriptor

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // File name
    const char *fileName = "testfile.txt";

    // Open the file for reading
    fd = open(fileName, O_RDONLY);


    if (fd == -1) {

        perror("Error opening file");

        return 1;

    }


    // Read from the file and write to the standard output
    while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {

        if (write(STDOUT_FILENO, buffer, bytesRead) == -1) {

            perror("Error writing to stdout");

            close(fd);

            return 1;

        }

    }


    if (bytesRead == -1) {

        perror("Error reading file");

    }


    // Close the file
    close(fd);

    return 0;

}