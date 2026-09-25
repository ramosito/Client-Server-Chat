#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>

int send_message(int socket, char* message);
int receive_message(int socket, char* message, size_t message_len);

#endif
