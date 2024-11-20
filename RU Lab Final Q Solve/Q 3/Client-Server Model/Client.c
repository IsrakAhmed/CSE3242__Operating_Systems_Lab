#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/wait.h>


#define PORT 8080
#define BUFFER_SIZE 1024

void *reader(void *client_socket) {
    int socket = *(int *)client_socket;

    while (1) {

        char buffer[BUFFER_SIZE] = {0};

        int bytes_read = read(socket, buffer, BUFFER_SIZE);

        if (bytes_read > 0) {
            //printf("Received: %s\n", buffer);
	    printf("%s\n", buffer);
        }

        // Check for the "GoodBye" command to break the loop
        if (strncmp(buffer, "GoodBye", 7) == 0) {
            exit(EXIT_SUCCESS);
        }

    }

    return NULL;

}


void *writer(void *client_socket) {
    
int socket = *(int *)client_socket;

    while (1) {

        char buffer[BUFFER_SIZE] = {0};

        //printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Remove newline character from input
        buffer[strcspn(buffer, "\n")] = 0;

        send(socket, buffer, strlen(buffer), 0);
        //printf("Sent: %s\n", buffer);

        // Check for the "GoodBye" command to break the loop
        if (strncmp(buffer, "GoodBye", 7) == 0) {
            exit(EXIT_SUCCESS);
        }

    }

    return NULL;

}


int main() {

    int sock = 0;

    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        perror("Socket creation error");

        return -1;

    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {

        perror("Invalid address / Address not supported");

        return -1;

    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {

        perror("Connection failed");

        return -1;

    }

    pthread_t reader_thread, writer_thread;

    // Pass the socket descriptor to the threads
    pthread_create(&reader_thread, NULL, reader, &sock);
    pthread_create(&writer_thread, NULL, writer, &sock);

    pthread_join(reader_thread, NULL);
    pthread_join(writer_thread, NULL);

    close(sock);

    return 0;

}