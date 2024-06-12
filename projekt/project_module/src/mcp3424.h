#include <stdint.h>

typedef struct
{
    int fileDescriptor;
    uint8_t address;
    uint8_t configuration;
} MCP3424;

void getVoltageFromAllChannels(float *result);
void initMCP3424();
