#ifndef CLIENT_ARRAY_H
#define CLIENT_ARRAY_H

#include <bits/pthreadtypes.h>
#include <stddef.h>

struct Client {
  int socket;
  pthread_t thread;
};

struct ClientArray {
  struct Client* clients;
  size_t size;
  size_t capacity;
};

struct ClientArray ca_init();
void ca_add(struct ClientArray* array, struct Client client);
void ca_remove(struct ClientArray* array, int socket);
void ca_destroy(struct ClientArray* array);

#endif
