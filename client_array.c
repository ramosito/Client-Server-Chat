#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "client_array.h"

struct ClientArray ca_init() {
    struct Client* clients = calloc(2, sizeof(*clients));
    if (clients == NULL) {
        perror("Error allocating client array");
        exit(1);
    }

    return (struct ClientArray) {
        .clients = clients,
        .size = 0,
        .capacity = 2,
    };
}

void ca_add(struct ClientArray* array, struct Client client) {
    if (array->size >= array->capacity) {
        array->clients = realloc(array->clients, 2 * array->capacity * sizeof(*array->clients));
        if (array->clients == NULL) {
            perror("Error allocating client array");
            exit(1);
        }

        array->capacity *= 2;
    }

    array->clients[array->size++] = client;
}

void ca_remove(struct ClientArray *array, int socket) {
    for (int i = 0; i < array->size; ++i) {
        if (array->clients[i].socket == socket) {
            array->clients[i] = array->clients[--array->size];
        }
    }
}

void ca_destroy(struct ClientArray* array) {
    free(array->clients);
    array->clients = NULL;
    array->size = 0;
    array->capacity = 0;
}
