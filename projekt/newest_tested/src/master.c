#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "server.h"
#include "mcp3424.h"

#define FIFO_PATH "/tmp/signal_to_led_fifo"

void send_fifo(float value) {
    int file;

    if ((file = open(FIFO_PATH, O_WRONLY)) < 0) {
        perror("Failed to open the FIFO");
        exit(1);
    }

    uint8_t buffer[sizeof(float)];
    memcpy(buffer, &value, sizeof(float));

    if (write(file, buffer, sizeof(float)) != sizeof(float)) {
        perror("Failed to write to the FIFO");
    }

    close(file);
}

int main() {
    char receiveBuffer[BUFFER_SIZE];
    initServer(8080, receiveBuffer);
    printf("Reveived message from the client: %s\n", receiveBuffer);
    // --- tutaj będą zapisywane arguemnty odczytane z joysticka w przyszłosci [TEMPORARY CHANGE]
    float reading[4] = {0.1, 0.4, 0.3, 0.2};
    // ---

    while (1) {
        // --- zmiana powinna następować na podstawie odczytu z czujnika [TEMPORARY CHANGE]
        if (fabs(3.3 - reading[0] <= 0.05)) {
            reading[0] = 0;
        }
        else {
            reading[0] += 0.1;
        }
        // ---

        printf("%f\n", reading[0]);
        fflush(stdout);

        send_fifo(reading[0]);
        sendData(reading, 16);

        usleep(500000);
    }

    return 0;
}
