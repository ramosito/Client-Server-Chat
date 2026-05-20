#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

void send_message(int socket, char* message);
void receive_message(int socket, char* message, size_t max_length);

#endif