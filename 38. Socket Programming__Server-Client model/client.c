#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return -1;
    }

    printf("Connected to the server. Type messages to send:\n");

    while (1) {
        // Get input from the user
        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send the message to the server
        if (write(sock, buffer, strlen(buffer)) == -1) {
            perror("Error writing to server");
            break;
        }

        // Read the server's response
        ssize_t bytes_read = read(sock, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null-terminate the received data
            printf("Server: %s", buffer);
        } else if (bytes_read == 0) {
            printf("Server disconnected.\n");
            break;
        } else {
            perror("Error reading from server");
            break;
        }
    }

    close(sock);  // Close the socket
    return 0;
}
