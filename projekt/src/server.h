#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>

void initServer(int port);
void sendData(const void *buffer, size_t size);

#endif
