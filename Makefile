CC = g++ -Wall -pedantic -ggdb -std=c++11

server:	server.cpp server.h
	$(CC) -o server server.cpp

client:	client.cpp client.h
	$(CC) -o client client.cpp

all:	server client
