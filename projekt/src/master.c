#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // for uint8_t
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h> // do abs
#include "server.h"

#define FIFO_PATH "/tmp/signal_to_led_fifo"  // Ścieżka do named pipe

// opkg install coreutils-mkfifo
// mkfifo /tmp/i2c_fifo


void send_fifo(float value) {
    int file;

    // Otwarcie named pipe do zapisu
    if ((file = open(FIFO_PATH, O_WRONLY)) < 0) {
        perror("Failed to open the FIFO");
        exit(1);
    }

    // Konwersja float na bajty
    uint8_t buffer[sizeof(float)];
    memcpy(buffer, &value, sizeof(float));

    // Wysłanie danych do named pipe
    if (write(file, buffer, sizeof(float)) != sizeof(float)) {
        perror("Failed to write to the FIFO");
    }

    close(file);
}

int main() {
    initServer(8080);
    float reading[4] = {0.1, 0.4, 0.3, 0.2};

    while (1) {
        if (fabs(3.3 - reading[0] <= 0.05)) {
            reading[0] = 0;
        }
        else {
            reading[0] += 0.1;
        }
        printf("%f\n", reading[0]);
        fflush(stdout);
        
        send_fifo(reading[0]);
        sendData(reading, 16);
        
        usleep(500000);
    }

    return 0;
}
