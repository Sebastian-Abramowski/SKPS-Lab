#include "server.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int socketFD;
struct sockaddr_in clientAddress;

void exitAndCleanup(int exitCode) {
    close(socketFD);
    exit(exitCode);
}

int createDatagramSocket(uint16_t port) {
    int sock;
    struct sockaddr_in serverAddress;

    // Create UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error: socket creation failed");
        exit(1);
    }

    // Configure server address structure
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the port
    if (bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error: bind failed");
        close(sock);
        exit(1);
    }

    return sock;
}

void waitForClient(char *buffer) {
    socklen_t clientAddressLen = sizeof(clientAddress);

    // Receive message from client
    if (recvfrom(socketFD, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddress, &clientAddressLen) < 0) {
        perror("Error: recvfrom failed");
        close(socketFD);
        exit(1);
    }

    // Send acknowledgment to client
    const char *ackMessage = "Ok";
    if (sendto(socketFD, ackMessage, strlen(ackMessage), 0, (struct sockaddr *)&clientAddress, clientAddressLen) < 0) {
        perror("Error: sendto failed");
        close(socketFD);
        exit(1);
    }
}

void initServer(int port, char *buffer) {
    socketFD = createDatagramSocket(port);
    waitForClient(buffer);
}

void sendData(const void *buffer, size_t size) {
    printf("Sending %zu bytes to %s\n", size, inet_ntoa(clientAddress.sin_addr));
    if (sendto(socketFD, buffer, size, 0, (struct sockaddr *)&clientAddress, sizeof(clientAddress)) < 0) {
        perror("Error: sendto response failed");
        close(socketFD);
        exit(1);
    }
}
