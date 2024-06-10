#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>

#define BUFFER_SIZE 5

void initServer(int port, char *buffer);
void sendData(const void *buffer, size_t size);

#endif
