CC = cc

CFLAGS = -Wall -Wextra -Wpedantic -std=c23

all: server client

rsa_crypto.o: rsa_crypto.c rsa_crypto.h
	$(CC) $(CFLAGS) -c rsa_crypto.c

common.o: common.c common.h
	$(CC) $(CFLAGS) -c common.c

client_array.o: client_array.c client_array.h
	$(CC) $(CFLAGS) -c client_array.c

server.o: server.c common.h client_array.h rsa_crypto.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c common.h rsa_crypto.h
	$(CC) $(CFLAGS) -c client.c

server: server.o common.o client_array.o rsa_crypto.o
	$(CC) $(CFLAGS) -o server server.o common.o client_array.o rsa_crypto.o

client: client.o common.o rsa_crypto.o
	$(CC) $(CFLAGS) -o client client.o common.o rsa_crypto.o

.PHONY: clean
clean:
	-rm -f server client server.o client.o common.o client_array.o rsa_crypto.o