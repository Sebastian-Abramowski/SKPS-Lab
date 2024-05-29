#include "server.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int                sockfd;
struct sockaddr_in client_addr;

void initServer() {
    struct sockaddr_in server_addr;

    socklen_t addr_len = sizeof(client_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    socklen_t clientAddressSize = sizeof(client_addr);
    int       connectionSocketFD;
    char      buffer[BUFFER_SIZE];

    if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                 (struct sockaddr *)&client_addr, &addr_len) < 0) {
        perror("recvfrom failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void sendData(const void *buffer, int size) {
    if (sendto(sockfd, buffer, size, 0,
               (const struct sockaddr *)&client_addr,
               sizeof(client_addr)) != size) {
        perror("sendto failed");
    }
}

int main() {
    initServer();
    char buffer[BUFFER_SIZE];
    while (1) {
        sleep(1);
        snprintf(buffer, 1024, "Wiadomość wysłana o %ld\n", time(NULL));
        sendData(buffer, strlen(buffer));
    }
}
