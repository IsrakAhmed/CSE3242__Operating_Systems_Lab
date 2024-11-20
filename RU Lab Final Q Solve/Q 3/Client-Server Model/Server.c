#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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


void handle_client(int client_socket) {

    pthread_t reader_thread, writer_thread;

    // Pass the socket descriptor to the threads
    pthread_create(&reader_thread, NULL, reader, &client_socket);
    pthread_create(&writer_thread, NULL, writer, &client_socket);

    pthread_join(reader_thread, NULL);
    pthread_join(writer_thread, NULL);

    close(client_socket);

}

int main() {

    int server_fd, new_socket;

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {

        perror("Socket failed");

        exit(EXIT_FAILURE);

    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {

        perror("Bind failed");

        close(server_fd);

        exit(EXIT_FAILURE);

    }

    if (listen(server_fd, 3) < 0) {
        
	perror("Listen failed");

        close(server_fd);

        exit(EXIT_FAILURE);

    }

    while (1) {

        printf("Waiting for a connection...\n");

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            
	    perror("Accept failed");

            close(server_fd);

            exit(EXIT_FAILURE);

        }

        printf("Connection accepted\n");

        pid_t pid = fork();

        if (pid < 0) {

            perror("Fork failed");

            close(new_socket);

        } else if (pid == 0) {

            close(server_fd);

            handle_client(new_socket);

            exit(0);

        } else {

            close(new_socket);

            waitpid(-1, NULL, WNOHANG);

        }
    }

    close(server_fd);

    return 0;

}