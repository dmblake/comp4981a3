CC = g++ -Wall -pedantic -g -std=c++11

server:	server.cpp server.h network.h
	$(CC) -o server server.cpp

client:	client.cpp client.h network.h
	$(CC) -o client client.cpp

all:	server client
