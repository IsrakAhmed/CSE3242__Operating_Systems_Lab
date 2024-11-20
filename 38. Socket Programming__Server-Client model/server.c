#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// Thread function to handle client communication
void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;
    free(client_socket);
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    printf("Client connected on socket %d\n", sock);

    // Communicate with the client
    while ((bytes_read = read(sock, buffer, sizeof(buffer))) > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the received data
        printf("Client %d: %s\n", sock, buffer);

        // Echo the message back to the client
        if (write(sock, buffer, bytes_read) == -1) {
            perror("Error writing to client");
            break;
        }
    }

    if (bytes_read == 0) {
        printf("Client %d disconnected.\n", sock);
    } else if (bytes_read == -1) {
        perror("Error reading from client");
    }

    close(sock);  // Close the client socket
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket, *client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create the server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address and bind the socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a new client connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Allocate memory for client socket and spawn a thread
        client_socket = malloc(sizeof(int));
        *client_socket = new_socket;
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_socket) != 0) {
            perror("Thread creation failed");
            free(client_socket);
            close(new_socket);
        }
    }

    close(server_fd);  // Close the server socket
    return 0;
}
