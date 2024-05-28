#include "mcp3424.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define RESOLUTION 12
#define RAW_MIN 0
#define RAW_MAX (1 << (RESOLUTION - 1))
#define V_MIN 0.00
#define V_MAX 3.33

MCP3424  properties;
uint32_t raw[4];

void handleError(const char *message) {
    perror(message);
    exit(1);
}

// configuration: bit 0  RDY; 1-2 channels; 3 mode; 4-5 resolution; 6-7 PGA

void setChannel(uint8_t channel) {
    // channels from 0-3
    properties.configuration &= ~0x60;
    properties.configuration |= (channel << 5);
}

void setConversionMode(uint8_t mode) {
    // mode: 0 - One shot, 1 - Continuous
    properties.configuration &= ~0x10;
    properties.configuration |= (mode << 4);
}

void setResolution(uint8_t resolution) {
    // resolution: 0 - 12-bit, 1 - 14-bit, 2 - 16-bit, 3 - 18-bit
    properties.configuration &= ~0x0c;
    properties.configuration |= (resolution << 2);
}

void setPGA(uint8_t PGA) {
    // PGA: 0 - 1x, 1 - 2x, 2 - 4x, 3 - 8x
    properties.configuration &= ~0x03;
    properties.configuration |= PGA;
}

void setI2CSlaveAddress() {
    if (ioctl(properties.fileDescriptor, I2C_SLAVE, properties.address) < 0) {
        handleError("ioctl error");
    }
}

void openI2CDevice() {
    properties.fileDescriptor = open("/dev/i2c-1", O_RDWR);
    if (properties.fileDescriptor < 0) {
        handleError("open error");
    }
}

void writeConfig() {
    if (write(properties.fileDescriptor, &properties.configuration, 1) <= 0) {
        handleError("write error");
    }
}

void readData(uint8_t *buffer, size_t length) {
    if (read(properties.fileDescriptor, buffer, length) <= 0) {
        handleError("read error");
    }
}

void initMCP3424() {
    openI2CDevice();
    properties.address = 0x68;
    properties.configuration = 0x00;
    setChannel(0);
    setConversionMode(1);
    setPGA(0);
    setResolution(0);
}

unsigned int getRaw(int channel) {
    setI2CSlaveAddress();
    setChannel(channel);
    writeConfig() uint8_t reading[4];
    while (1) {
        readData(reading, 4);
        // Check if data is ready to read
        if ((reading[2] >> 7) == 0) {
            break;
        }
    }

    unsigned int raw = ((reading[0] & 0x0f) << 8) | reading[1];
    return raw;
}

float getVoltageFromRaw(int val) {
    // input voltage = output * 2 * Vref / 2**(resolution) / PGA
    return ((val - RAW_MIN) * ((V_MAX - V_MIN) / (RAW_MAX - RAW_MIN))) + V_MIN;
}

void getVoltageFromAllChannels(float *result) {
    for (int i = 0; i < 4; ++i) {
        raw[i] = getRaw(i);
        result[i] = getVoltageFromRaw(raw[i]);
    }
}
