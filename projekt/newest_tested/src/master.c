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
    initializeMCP3424();
    initServer(8080, receiveBuffer);
    printf("Reveived message from the client: %s\n", receiveBuffer);
    float readings[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    while (1) {
        // --- zmiana powinna następować na podstawie odczytu z czujnika [TEMPORARY CHANGE]
        // if (fabs(3.3 - readings[0] <= 0.05)) {
        //     readings[0] = 0;
        // }
        // else {
        //     readings[0] += 0.1;
        // }
        // ---
        getReading(readings);

        printf("%f, %f, %f, %f\n", readings[0], readings[1], readings[2], readings[3]);
        fflush(stdout);

        send_fifo(readings[0]);
        sendData(readings, 16);

        usleep(250000);
    }

    return 0;
}
