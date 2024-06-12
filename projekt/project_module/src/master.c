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

void send_fifo(float *values)
{
    int file;

    if ((file = open(FIFO_PATH, O_WRONLY)) < 0)
    {
        perror("Failed to open the FIFO");
        exit(1);
    }

    uint8_t buffer[4 * sizeof(float)];
    memcpy(buffer, values, 4 * sizeof(float));

    if (write(file, buffer, 4 * sizeof(float)) != 4 * sizeof(float))
    {
        perror("Failed to write to the FIFO");
    }

    close(file);
}

int main()
{
    char receiveBuffer[BUFFER_SIZE];
    initMCP3424();
    initServer(8080, receiveBuffer);
    printf("Reveived message from the client: %s\n", receiveBuffer);
    float readings[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    while (1)
    {
        // --- [FOR TESTING]
        // if (fabs(3.3f - readings[0] <= 0.05f)) {
        //     readings[0] = 0;
        // }
        // else {
        //     readings[0] += 0.1f;
        // }

        // if (readings[2] >= 0) {
        //     readings[2] -= 0.1f;
        // }
        // else {
        //     readings[2] = 3.3f;
        // }
        // ---
        getVoltageFromAllChannels(readings);

        printf("%f, %f, %f, %f\n", readings[0], readings[1], readings[2], readings[3]);
        fflush(stdout);

        send_fifo(readings);
        sendData(readings, 16);

        usleep(250000);
    }

    return 0;
}
