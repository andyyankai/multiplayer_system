all: client server

client: client.o 
	gcc -o client client.o

server: server.o 
	gcc -o server server.o

client.o: client.c port.h

server.o: server.c port.h

clean:
	rm -f client server client.o server.o 
