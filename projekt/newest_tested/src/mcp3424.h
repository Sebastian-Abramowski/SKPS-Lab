#include <stdint.h>

typedef struct {
  int fileDescriptor;
  uint8_t address;
  uint8_t config;
} MCP3424Properties;

void getReading(float *result);
void initializeMCP3424();
