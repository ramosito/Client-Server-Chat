#include "client_array.h"
#include <stdlib.h>

struct ClientArray ca_init()
{
    struct ClientArray array;
    array.size     = 0;
    array.capacity = 2;
    array.clients  = malloc(array.capacity * sizeof(struct Client));
    return array;
}

void ca_add(struct ClientArray* array, struct Client client)
{
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->clients = realloc(array->clients, array->capacity * sizeof(struct Client));
    }
    array->clients[array->size] = client;
    array->size++;
}