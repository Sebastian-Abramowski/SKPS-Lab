#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "mcp3424.h"

#define RESOLUTION 12
#define RAW_MIN 0
#define RAW_MAX 4095
#define V_MIN 0.00
#define V_MAX 3.33

MCP3424Properties properties;
uint32_t raw[4];

void setChannel(int channel) {
    properties.config &= ~0x60;
    properties.config |= (channel << 5);
}

void setConversionModeContinous() {
    properties.config &= ~0x10;
    properties.config |= (1 << 4);
}

void setPGA1X() {
    properties.config &= ~0x03;
    properties.config |= 0;
}

void setResolution12() {
    properties.config &= ~0x0c;
    properties.config |= (0 << 2);
}

void initializeMCP3424() {
    properties.fileDescriptor = open("/dev/i2c-1", O_RDWR);
    properties.address = 0x68;
    properties.config = 0x00;
    setChannel(0);
    setConversionModeContinous();
    setPGA1X();
    setResolution12();
}

unsigned int getRawReading(int channel) {
    if (ioctl(properties.fileDescriptor, I2C_SLAVE, properties.address) < 0) {
        perror("(mcp3424) ioctl");
        exit(1);
    }

    setChannel(channel);

    ssize_t n = write(properties.fileDescriptor, &properties.config, 1);
    if (n <= 0) {
        perror("(mcp3424) write");
        exit(1);
    }

    uint8_t reading[4];
    while(1) {
        n = read(properties.fileDescriptor, reading, 4);
        if (n <= 0) {
            perror("(mcp3424) read");
            exit(1);
        }

        if ((reading[2] >> 7) == 0) {
            break;
        }
    }

    unsigned int raw = ((reading[0] & 0x0f) << 8) | reading[1];
    return raw;
}

float getVoltageFromRaw(int val) {
    return ((val - RAW_MIN) * ((V_MAX - V_MIN) / (RAW_MAX - RAW_MIN))) + V_MIN;
}

void getReading(float *result) {
    for (int i = 0; i < 4; ++i) {
        raw[i] = getRawReading(i);
        result[i] = getVoltageFromRaw(raw[i]);
    }
}
