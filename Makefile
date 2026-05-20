common.o: common.c common.h
	gcc -c common.c

client_array.o: client_array.c client_array.h
	gcc -c client_array.c

server.o: server.c common.h client_array.h
	gcc -c server.c

client.o: client.c common.h
	gcc -c client.c

server: server.o common.o client_array.o
	gcc -o server server.o common.o client_array.o -lpthread

client: client.o common.o
	gcc -o client client.o common.o -lpthread